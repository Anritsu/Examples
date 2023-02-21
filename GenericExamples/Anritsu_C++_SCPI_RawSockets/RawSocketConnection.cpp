#include <iostream>
#include "RawSocketConnection.h"
#include "CustomException.h"

RawSocketConnection::RawSocketConnection(std::string resourceName) : SCPIConnection(resourceName)
{
	std::string tempResourceName = ResourceName.substr(ResourceName.find("::") + 2);
	PortNo = atoi(tempResourceName.substr(tempResourceName.find("::") + 2,
		tempResourceName.rfind("::") - tempResourceName.find("::") + 2).c_str());
	strcpy_s(IpAddress, tempResourceName.substr(0, tempResourceName.find("::")).c_str());

}

RawSocketConnection::~RawSocketConnection()
{
}

void RawSocketConnection::Connect() 
{
	try {
		if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
		{
			throw SocketConnectionConnectError("WSAStartup Failed. Error Code: " + std::to_string(WSAGetLastError()) + " - at " + ResourceName + ".");
			return;
		}

		//Create a socket
		if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		{
			throw SocketConnectionConnectError("Could not create socket. Error Code: " + std::to_string(WSAGetLastError()) + ".");
			return;
		}

		Server.sin_addr.s_addr = inet_addr(IpAddress);
		Server.sin_family = AF_INET;
		Server.sin_port = htons(PortNo);

		//Connect to remote server
		if (connect(Socket, (struct sockaddr*)&Server, sizeof(Server)) < 0)
		{
			throw SocketConnectionConnectError("Socket connection error. Error Code: " + std::to_string(WSAGetLastError()) + ".");
			return;
		}
		Connected = true;
	}
	catch (SocketConnectionConnectError& scce)
	{
		std::cout << scce.toString() << std::endl;
		throw scce;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to create socket connection to " << ResourceName << "." << std::endl;
		throw e;
	}
}

void RawSocketConnection::SetTimeout(int timeOutMs)
{
	try
	{
		if (Connected)
		{
			int retCode = setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeOutMs, sizeof(timeOutMs));
			if (retCode == SOCKET_ERROR) 
			{
				throw SocketTimeoutSetReceiveError("Failed to set receive timeout. Error Code: " + std::to_string(WSAGetLastError()) + ".");
			}
			retCode = setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeOutMs, sizeof(timeOutMs));
			if (retCode == SOCKET_ERROR) 
			{
				throw SocketTimeoutSetSendError("Failed to set send timeout. Error Code: " + std::to_string(WSAGetLastError()) + ".");
			}
		}
		else
		{
			throw SocketConnectionError("Failed to set timeout! Not connected to instrument " + ResourceName + ".");
		}
	}
	catch (SocketTimeoutSetReceiveError& stsre)
	{
		std::cout << stsre.toString() << std::endl;
		throw stsre;
	}
	catch (SocketTimeoutSetSendError& stsse)
	{
		std::cout << stsse.toString() << std::endl;
		throw stsse;
	}
	catch (SocketConnectionError& sce)
	{
		std::cout << sce.toString() << std::endl;
		throw sce;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to set connection timeout to " << ResourceName << "." << std::endl;
		throw e;
	}
}
void RawSocketConnection::Disconnect() 
{
	try {
		int closedstatus = closesocket(Socket);
		if (closedstatus != 0)
		{
			throw SocketConnectionDisconnectError("Closing socket connection failed with return code " + std::to_string(closedstatus) + ".");
		}
		Connected = false;
	}
	catch (SocketConnectionDisconnectError& scde)
	{
		std::cout << scde.toString() << std::endl;
		throw scde;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to disconnect from socket connection at " << ResourceName << "." << std::endl;
		throw e;
	}
}

int RawSocketConnection::Send(std::string command) 
{ 
	try {
		if (Connected)
		{
			char buffer[ReadSize];
			memset(buffer, 0, ReadSize);
			sprintf_s(buffer, "%s\n", command.c_str());

			int retCount = send(Socket, buffer, strlen(buffer), 0);
			if (retCount < 0)
			{
				throw SocketCommunicationWriteError("Error when sending the command: '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
			}
		}
		else
		{
			throw SocketConnectionError("Failed to send command: '" + command + "'! Not connected to instrument " + ResourceName + ".");
		}
		return 0;
	}
	catch (SocketCommunicationWriteError& scwe)
	{
		std::cout << scwe.toString() << std::endl;
		throw scwe;
	}
	catch (SocketConnectionError& sce)
	{
		std::cout << sce.toString() << std::endl;
		throw sce;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to write to socket connection at " << ResourceName << "." << std::endl;
		throw e;
	}
}

std::string RawSocketConnection::Query(std::string command) 
{ 
	try
	{
		if (Connected)
		{
			char buffer[ReadSize];
			memset(buffer, 0, ReadSize);
			sprintf_s(buffer, "%s\n", command.c_str());

			Send(command);
			unsigned int retCount = 0;
			char respBuf[ReadSize];
			memset(respBuf, 0, ReadSize);
			retCount = recv(Socket, respBuf, SCPIConnection::OneByte, 0);

			if (retCount > 0)
			{
				if (retCount == 0xFFFFFFFF)
				{
					int error_code = WSAGetLastError();
					if (error_code == 10060)
					{
						throw SocketCommunicationTimeoutError("Command execution for command '" + command + "' not finished.");
					}
				}

				if ((char)respBuf[0] != '#')
				{
					std::string returnValue((char*)respBuf, 1);
					memset(respBuf, 0, ReadSize);
					retCount = recv(Socket, respBuf, ReadSize, 0);

					if (retCount > 0)
					{
						if (retCount == 0xFFFFFFFF)
						{
							int error_code = WSAGetLastError();
							if (error_code == 10060)
							{
								throw SocketCommunicationTimeoutError("Command execution for command '" + command + "' not finished.");
							}
						}
						returnValue.append(std::string(respBuf, retCount));
						returnValue.pop_back();
						return returnValue;
					}
					else if (retCount == 0)
					{
						throw SocketConnectionError("Error when reading response from instrument to command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
					}
					else
					{
						throw SocketCommunicationReadError("Error when reading response from instrument to command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
					}
				}
				else
				{
					memset(respBuf, 0, ReadSize);
					retCount = recv(Socket, respBuf, 1, 0);

					if (retCount < 1)
					{
						if (retCount == 0)
						{
							throw SocketConnectionError("Error when reading buffer size number of digits for command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
						}
						else
						{
							throw SocketCommunicationReadError("Error when reading buffer size number of digits for command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
						}
					}

					uint32_t readBuffDigits = respBuf[0] - '0';

					memset(respBuf, 0, ReadSize);
					retCount = recv(Socket, respBuf, readBuffDigits, 0);

					if (retCount == 0xFFFFFFFF)
					{
						int error_code = WSAGetLastError();
						if (error_code == 10060)
						{
							throw SocketCommunicationTimeoutError("Command execution for command '" + command + "' not finished.");
						}
					}

					if (retCount < readBuffDigits)
					{
						throw SocketCommunicationReadError("Error when reading buffer size for command '" + command + "'! Return count size : " + std::to_string(retCount) + ".");
					}

					int bufferSize = atoi(respBuf);
					memset(respBuf, 0, ReadSize);
					retCount = recv(Socket, respBuf, ReadSize, 0);

					if (retCount == 0xFFFFFFFF)
					{
						int error_code = WSAGetLastError();
						if (error_code == 10060)
						{
							throw SocketCommunicationTimeoutError("Command execution for command '" + command + "' not finished.");
						}
					}

					if (retCount <= 0)
					{
						if (retCount == 0)
						{
							throw SocketConnectionError("Error when reading block data for command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
						}
						else
						{
							throw SocketCommunicationReadError("Error when reading block data for command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
						}
					}

					std::string rtn(respBuf, retCount);
					memset(respBuf, 0, ReadSize);
					int alreadyRead = retCount;
					while (alreadyRead < bufferSize)
					{
						retCount = recv(Socket, respBuf, ReadSize, 0);

						if (retCount == 0xFFFFFFFF)
						{
							int error_code = WSAGetLastError();
							if (error_code == 10060)
							{
								throw SocketCommunicationTimeoutError("Command execution for command '" + command + "' not finished.");
							}
						}

						if (retCount <= 0)
						{
							if (retCount == 0)
							{
								throw SocketConnectionError("Error when reading block data for command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
							}
							else
							{
								throw SocketCommunicationReadError("Error when reading block data for command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
							}
						}
						alreadyRead += retCount;
						rtn.append(std::string(respBuf, retCount));
						memset(respBuf, 0, ReadSize);
					}
					rtn.pop_back();
					return rtn;

				}
			}
			else if (retCount == 0)
			{
			throw SocketConnectionError("Error when reading first character for command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
			}
			else
			{
			throw SocketCommunicationReadError("Error when reading first character for command '" + command + "'! Return count size: " + std::to_string(retCount) + ".");
			}
		}
		else
		{
			throw SocketConnectionError("Failed to send query! Not connected to instrument " + ResourceName + ".");
		}
	}
	catch (SocketCommunicationTimeoutError& scte)
	{
		std::cout << scte.toString() << std::endl;
		throw scte;
	}
	catch (SocketCommunicationReadError& scre)
	{
		std::cout << scre.toString() << std::endl;
		throw scre;
	}
	catch (SocketConnectionError& sce)
	{
		std::cout << sce.toString() << std::endl;
		throw sce;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to read from socket connection at " << ResourceName << "." << std::endl;
		throw e;
	}
}