#include "VXI11Connection.h"
#include "CustomException.h"
#include <iostream>



VXI11Connection::VXI11Connection(std::string resourceName) : SCPIConnection(resourceName)
{
}

VXI11Connection::~VXI11Connection()
{
}

void VXI11Connection::SetTimeout(int timeOutMs)
{
	if (Connected)
	{
		viSetAttribute(Session, VI_ATTR_TMO_VALUE, timeOutMs);
	}
	else
	{
		throw CustomException((" Failed to set timeout! Not connected to instrument!" +
			ResourceName));
	}
}

void VXI11Connection::Connect()
{
	viOpenDefaultRM(&ResourceManager);
	ViStatus status = viOpen(ResourceManager, ResourceName.c_str(), 
		VI_NULL, VI_NULL, &Session);
	if (status != VI_SUCCESS)
	{
		viClose(ResourceManager);
		throw CustomException(("Failed to connect to " + ResourceName));
	}	
	Connected = true;
}

void VXI11Connection::Disconnect()
{
	viClose(Session);
	viClose(ResourceManager);
	Connected = false;
}
int VXI11Connection::Send(std::string command)
{
	if (Connected) 
	{
		ViStatus status = viPrintf(Session, (command + "\n").c_str());
		if (status != VI_SUCCESS) 
		{
			throw CustomException(("Error when trying to write command " + 
				command +
				" Return STATUS: " + 
				std::to_string(status)));
		}
	}
	else 
	{
		throw CustomException(("Not connected to instrument " +
			ResourceName));
	}
	return VI_SUCCESS;
}
std::string VXI11Connection::Query(std::string command)
{
	if (Connected)
	{
		Send(command);;
		char respBuf[ReadSize] = { 0 };
		ViUInt32 retCount = 0;
		ViStatus status = VI_SUCCESS;

		status = viRead(Session, (ViBuf)respBuf, SCPIConnection::OneByte, &retCount);
		if (retCount == SCPIConnection::OneByte)
		{
			if ((char)respBuf[0] != '#')
			{
				std::string returnValue((char *)respBuf, SCPIConnection::OneByte);
				memset(respBuf, 0, ReadSize);
				status = viScanf(Session, "%t", respBuf);
				if (strlen(respBuf) > 0) {
					returnValue.append(std::string(respBuf, strlen(respBuf)));
					returnValue.pop_back();
					return returnValue;
				}
				else
				{
					throw CustomException(("Error when reading response from instrument to command" +
						command + 
						" Return count size: " + 
						std::to_string(retCount)) + 
						" Return STATUS: " +
						std::to_string(status));
				}
			}
			else
			{
				status = viRead(Session, (ViBuf)respBuf, SCPIConnection::OneByte, &retCount);
				if (retCount != SCPIConnection::OneByte)
				{
					throw CustomException(("Error when reading buffer size number of digits! Return count size: " + 					
						std::to_string(retCount)) + 
						" Return STATUS: " +
						std::to_string(status));
				}
				uint32_t readBuffDigits = respBuf[0] - '0';

				memset(respBuf, 0, ReadSize);
				status = viRead(Session, (ViBuf)respBuf, readBuffDigits, &retCount);
				if (retCount < readBuffDigits)
				{
					throw CustomException(("Error when reading buffer size! Return count size: " + 
						std::to_string(retCount))+ 
						" Return STATUS: " +
						std::to_string(status));
				}

				int bufferSize = atoi(respBuf);
				memset(respBuf, 0, ReadSize);
				viRead(Session, (ViBuf)respBuf, ReadSize, &retCount);
				if (retCount == 0)
				{
					throw CustomException(("Error when reading block data! Return count size: "	+
						std::to_string(retCount)) + 
						" Return STATUS: " +
						std::to_string(status));
				}

				std::string returnValue(respBuf, retCount);
				memset(respBuf, 0, ReadSize);
				int alreadyRead = retCount;
				while (alreadyRead < bufferSize)
				{
					status = viRead(Session, (ViBuf)respBuf, ReadSize, &retCount);
					if (retCount == 0)
					{
						throw CustomException(("Error when reading block data! Return count size: " +
							std::to_string(retCount)) + 
							" Return STATUS: " +
							std::to_string(status));
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
			throw CustomException(("Error when reading first character! Return count size: " +
				std::to_string(retCount)) + 
				" Return STATUS: " +
				std::to_string(status));
		}
	} 
	else 
	{
		throw CustomException(("Not connected to instument " + ResourceName));
	}
	return "";
}
