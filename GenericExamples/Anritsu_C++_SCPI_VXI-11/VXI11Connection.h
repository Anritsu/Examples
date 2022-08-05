#pragma once
#include "SCPIConnection.h"
#include "visa.h"
class VXI11Connection :
	public SCPIConnection
{
public:
	VXI11Connection(std::string resourceName);
	~VXI11Connection();

	void Connect();
	void Disconnect();
	int Send(std::string cmd);
	std::string Query(std::string cmd);
	void SetTimeout(int timeOutMs);
	
private:
	ViSession ResourceManager;
	ViSession Session;
};
