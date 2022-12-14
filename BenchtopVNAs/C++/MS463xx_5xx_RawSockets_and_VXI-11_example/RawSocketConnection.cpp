#include <iostream>
#include "RawSocketConnection.h"
#include "CustomException.h"

RawSocketConnection::RawSocketConnection(std::string resourceName) : SCPIConnection(resourceName)
{
	std::string tempResourceName = ResourceName.substr(ResourceName.find("::") + 2);
	PortNo = atoi(tempResourceName.substr(tempResourceName.find(":") + 2,
		tempResourceName.rfind("::") - tempResourceName.find("::") + 2).c_str());
	strcpy_s(IpAddress, tempResourceName.substr(0, tempResourceName.find("::")).c_str());
}

RawSocketConnection::~RawSocketConnection()
{
}

void RawSocketConnection::Connect()
{
	if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
	{
		throw CustomException("WSAStartup Failed. Error Code: " + std::to_string(WSAGetLastError()));
		return;
	}

	if ((Socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		throw CustomException("Could not create socket: " + std::to_string(WSAGetLastError()));
		return;
	}
	inet_pton(AF_INET, IpAddress, &Server.sin_addr.s_addr);
	Server.sin_family = AF_INET;
	Server.sin_port = htons(PortNo);

	if (connect(Socket, (struct sockaddr*)&Server, sizeof(Server)) < 0)
	{
		throw CustomException("Connection error!");
		return;
	}
	Connected = true;
}

void RawSocketConnection::SetTimeout(int timeout)
{
	if (Connected)
	{
		int retCode = setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
		if (retCode == SOCKET_ERROR) {
			throw CustomException("Failed to set receive timeout");
		}
		retCode = setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
		if (retCode == SOCKET_ERROR) {
			throw CustomException("Failed to set send timeout");
		}
	}
	else
	{
		throw CustomException("Failed to set timeout! Not connected to instrument " + ResourceName);
	}
}

void RawSocketConnection::Disconnect()
{
	closesocket(Socket);
	Connected = false;
}

int RawSocketConnection::Write(std::string w_command)
{
	if (Connected)
	{
		char buffer[ReadSize];
		memset(buffer, 0, ReadSize);
		sprintf_s(buffer, "%s\n", w_command.c_str());

		int retCount = send(Socket, buffer, strlen(buffer), 0);
		if (retCount < 0)
		{
			throw CustomException("Error when writing to socket command: " + w_command + "! Return count size: " + std::to_string(retCount));
		}
	}
	else
	{
		throw CustomException("Failed to send command! Not connected to instrument " + ResourceName);
	}
}

std::string RawSocketConnection::Query(std::string q_command)
{
	if (Connected)
	{
		char buffer[ReadSize];
		memset(buffer, 0, ReadSize);
		sprintf_s(buffer, "%s\n", q_command.c_str());

		Write(q_command);
		unsigned int retCount = 0;
		char respBuf[ReadSize];
		memset(respBuf, 0, ReadSize);
		retCount = recv(Socket, respBuf, SCPIConnection::OneByte, 0);
		if (retCount > 0)
		{
			if ((char)respBuf[0] != '#')
			{
				std::string returnValue((char*)respBuf, 1);
				memset(respBuf, 0, ReadSize);
				retCount = recv(Socket, respBuf, ReadSize, 0);
				if (retCount > 0)
				{
					returnValue.append(std::string(respBuf, retCount));
					returnValue.pop_back();
					return returnValue;
				}
				else
				{
					throw CustomException("Error when reading response from instrument to command " + q_command + "! Return count size: " + std::to_string(retCount));
				}
			}
			else
			{
				memset(respBuf, 0, ReadSize);
				retCount = recv(Socket, respBuf, 1, 0);
				if (retCount < 1)
				{
					throw CustomException("Error when reading buffer size number of digits! Return count size: " + std::to_string(retCount));
				}
				uint32_t readBuffDigits = respBuf[0] - '0';

				memset(respBuf, 0, ReadSize);
				retCount = recv(Socket, respBuf, readBuffDigits, 0);
				if (retCount < readBuffDigits)
				{
					throw CustomException("Error when reading buffer size! return count size: " + std::to_string(retCount));
				}

				int bufferSize = atoi(respBuf);
				memset(respBuf, 0, ReadSize);
				retCount = recv(Socket, respBuf, ReadSize, 0);
				if (retCount <= 0)
				{
					throw CustomException("Error when reading block data! Return count size: " + std::to_string(retCount));
				}

				std::string rtn(respBuf, retCount);
				memset(respBuf, 0, ReadSize);
				int alreadyRead = retCount;
				while (alreadyRead < bufferSize)
				{
					retCount = recv(Socket, respBuf, ReadSize, 0);
					if (retCount <= 0)
					{
						throw CustomException("Error when reading block data! Return count size: " + std::to_string(retCount));
					}
					alreadyRead += retCount;
					rtn.append(std::string(respBuf, retCount));
					memset(respBuf, 0, ReadSize);
				}
				rtn.pop_back();
				return rtn;
			}
		}
		else
		{
			throw CustomException("Error when reading first character! Return count size: " + std::to_string(retCount));
		}
	}
	else
	{
		throw CustomException("Failed to send query! Not connected to instrument " + ResourceName);
	}
}