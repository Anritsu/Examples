#pragma once
#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "SCPIConnection.h"
class RawSocketConnection :
	public SCPIConnection
{
public:
	RawSocketConnection(std::string resourceName);
	~RawSocketConnection();

	void Connect();
	void Disconnect();
	int Send(std::string command);
	std::string Query(std::string command);
	void SetTimeout(int timeOutMs);

private:
	WSADATA WSA;
	SOCKET Socket;
	struct sockaddr_in Server;

	int PortNo;
	char IpAddress[256];	
};

