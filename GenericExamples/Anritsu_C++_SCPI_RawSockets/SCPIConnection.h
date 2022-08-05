#pragma once
#include <string>

class SCPIConnection
{
public:
	SCPIConnection(std::string resourceName);
	virtual ~SCPIConnection();
	virtual void Connect() = 0;
	virtual void Disconnect() = 0;
	virtual int Send(std::string cmd) = 0;
	virtual std::string Query(std::string command) = 0;
	virtual void SetTimeout(int timeOutMs) = 0;

protected:
	std::string ResourceName;
	bool Connected;
	const static int ReadSize = 4096;
	const static int OneByte = 1;
};

