#include "VXIConnection.h"
#include "CustomException.h"
#include <iostream>

VXIConnection::VXIConnection(std::string resourceName) : SCPIConnection(resourceName)
{
}

VXIConnection::~VXIConnection()
{
}

void VXIConnection::SetTimeout(int timeout)
{
	if (Connected)
	{
		ViStatus status = viSetAttribute(Session, VI_ATTR_TMO_VALUE, timeout);
		std::cout << status << std::endl;
	}
	else
	{
		throw VisaTimeoutSetError("Failed to set timeout! Not connected to instrument " + ResourceName);
	}
}

void VXIConnection::Connect()
{
	try {
		viOpenDefaultRM(&ResourceManager);
		ViStatus status = viOpen(ResourceManager, ResourceName.c_str(), VI_NULL, VI_NULL, &Session);
		if (status != VI_SUCCESS)
		{
			viClose(ResourceManager);
			throw VisaConnectionConnectError("Failed to connect to " + ResourceName);
		}
		Connected = true;
	}
	catch (VisaConnectionConnectError& vcce)
	{
		std::cout << vcce.toString() << ": " << vcce.what() << std::endl;
		throw vcce;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to create VXI-11 connection to " << ResourceName << std::endl;
		throw e;
	}
}

void VXIConnection::Disconnect()
{
	try
	{
		ViStatus closeSessionStatus = viClose(Session);
		if (closeSessionStatus != 0)
		{
			throw VisaConnectionDisconnectError("Failed to disconnect from " + ResourceName);
		}
		ViStatus closeResourceStatus = viClose(ResourceManager);
		if (closeResourceStatus != 0)
		{
			throw VisaConnectionDisconnectError("Failed to close the visa resource manager");
		}
		Connected = false;
	}
	catch (VisaConnectionDisconnectError& vcde)
	{
		std::cout << vcde.toString() << ": " << vcde.what() << std::endl;
		throw vcde;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to create disconnect from " << ResourceName << std::endl;
		throw e;
	}
}
int VXIConnection::Write(std::string w_command)
{
	try {
		if (Connected)
		{
			ViStatus status = viPrintf(Session, (w_command + "\n").c_str());
			if (status != VI_SUCCESS)
			{
				throw VisaCommunicationWriteError("Error when trying to write command " + w_command + ". Return STATUS: " + std::to_string(status));
			}
		}
		else
		{
			throw VisaConnectionError("Failed to send command : '" + w_command + "'!Not connected to instrument " + ResourceName);
		}
		return VI_SUCCESS;
	}
	catch (VisaCommunicationWriteError& vcwe)
	{
		std::cout << vcwe.toString() << "; " << vcwe.what() << std::endl;
		throw vcwe;
	}
	catch (VisaConnectionError& vce)
	{
		std::cout << vce.toString() << ": " << vce.what() << std::endl;
		throw vce;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to write to socket connection at " << ResourceName << std::endl;
		throw e;
	}
}
std::string VXIConnection::Query(std::string q_command)
{
	try {
		if (Connected)
		{
			Write(q_command);
			char respBuf[ReadSize] = { 0 };
			ViUInt32 retCount = 0;
			ViStatus status = VI_SUCCESS;
			status = viRead(Session, (ViBuf)respBuf, SCPIConnection::OneByte, &retCount);
			if (status == VI_ERROR_TMO)
			{
				throw VisaCommunicationTimeoutError("Command execution for command '" + q_command + "' not finished");
			}
			else if (((status < 1073676290) && (status != 0)) || (status > 1073676443))
			{
				throw VisaCommunicationReadError("Error when reading the first character of the response from instrument to command '" + q_command + "'!Visa error status : " + std::to_string(status));
			}
			if (retCount == SCPIConnection::OneByte)
			{
				if ((char)respBuf[0] != '#')
				{
					std::string returnValue((char*)respBuf, SCPIConnection::OneByte);
					memset(respBuf, 0, ReadSize);
					status = viScanf(Session, "%t", respBuf);
					if (status != 0)
					{
						throw VisaCommunicationReadError("Error when reading response from instrument to command " + q_command +
							". Return count size: " + std::to_string(retCount) +
							". Return STATUS: " + std::to_string(status) + ".");
					}
					else
					{
						returnValue.append(std::string(respBuf, strlen(respBuf)));
						returnValue.pop_back();
						return returnValue;
					}
				}
				else
				{
					status = viRead(Session, (ViBuf)respBuf, SCPIConnection::OneByte, &retCount);
					if (status == VI_ERROR_TMO)
					{
						throw VisaCommunicationTimeoutError("Command execution for command '" + q_command + "' not finished");
					}
					else if (((status < 1073676290) && (status != 0)) || (status > 1073676443) || (retCount != SCPIConnection::OneByte))
					{
						throw VisaCommunicationReadError("Error when reading buffer size number of digits! Return count size: " +
							std::to_string(retCount) +
							". Return STATUS: " + std::to_string(status) + ".");
					}

					uint32_t readBuffDigits = respBuf[0] - '0';

					memset(respBuf, 0, ReadSize);
					status = viRead(Session, (ViBuf)respBuf, readBuffDigits, &retCount);

					if (status == VI_ERROR_TMO)
					{
						throw VisaCommunicationTimeoutError("Command execution for command '" + q_command + "' not finished");
					}
					else if (((status < 1073676290) && (status != 0)) || (status > 1073676443) || (retCount < readBuffDigits))
					{
						throw VisaCommunicationReadError("Error when reading buffer size number of digits! Return count size: " +
							std::to_string(retCount) +
							". Return STATUS: " + std::to_string(status) + ".");
					}
					int bufferSize = atoi(respBuf);

					memset(respBuf, 0, ReadSize);
					status = viRead(Session, (ViBuf)respBuf, ReadSize, &retCount);
					std::cout << status << std::endl;

					if (status == VI_ERROR_TMO)
					{
						throw VisaCommunicationTimeoutError("Command execution for command '" + q_command + "' not finished");
					}
					else if (((status < 1073676290) && (status != 0)) || (status > 1073676443) || (retCount == 0))
					{
						throw VisaCommunicationReadError("Error when reading block data! Return count size: " +
							std::to_string(retCount) +
							". Return Status: " +
							std::to_string(status) + ".");
					}
					std::string returnValue(respBuf, retCount);

					memset(respBuf, 0, ReadSize);
					int alreadyRead = retCount;
					while (alreadyRead < bufferSize)
					{
						status = viRead(Session, (ViBuf)respBuf, ReadSize, &retCount);
						std::cout << status << std::endl;

						if (status == VI_ERROR_TMO)
						{
							throw VisaCommunicationTimeoutError("Command execution for command '" + q_command + "' not finished");
						}
						else if (((status < 1073676290) && (status != 0)) || (status > 1073676443) || (retCount == 0))
						{
							throw VisaCommunicationReadError("Error when reading block data! Return count size: " +
								std::to_string(retCount) + ". Return Status: " +
								std::to_string(status) + ".");
						}

						alreadyRead += retCount;
						returnValue.append(std::string(respBuf, retCount));
						memset(respBuf, 0, ReadSize);
					}
					returnValue.pop_back();
					return returnValue;
				}
			}
			else
			{
				std::cout << std::to_string(retCount) << std::to_string(status) << std::endl;
				throw VisaCommunicationReadError("Error when reading first character! Return count size: " +
					std::to_string(retCount) +
					". Return STATUS: " +
					std::to_string(status) + ".");
			}
		}
		else
		{
			throw VisaConnectionError("Failed to send query! Not connected to instrument " + ResourceName);
		}
	}
	catch (VisaCommunicationTimeoutError& vcte)
	{
		std::cout << vcte.toString() << ": " << vcte.what() << std::endl;
		throw vcte;
	}
	catch (VisaCommunicationReadError& vcre)
	{
		std::cout << vcre.toString() << ": " << vcre.what() << std::endl;
		throw vcre;
	}
	catch (VisaConnectionError& vce)
	{
		std::cout << vce.toString() << ": " << vce.what() << std::endl;
		throw vce;
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected exception: " << e.what() << " when trying to read from socket connection at " << ResourceName << std::endl;
		throw e;
	}
}