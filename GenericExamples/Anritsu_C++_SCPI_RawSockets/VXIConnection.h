#pragma once
#include "SCPIConnection.h"
#include "visa.h"
class VXIConnection :
	public SCPIConnection
{
public:
	VXIConnection(std::string resourceName);
	~VXIConnection();

	void Connect();
	void Disconnect();
	int Send(std::string cmd);
	std::string Query(std::string cmd);
	void SetTimeout(int timeOutMs);
	
private:
	ViSession ResourceManager;
	ViSession Session;
};
