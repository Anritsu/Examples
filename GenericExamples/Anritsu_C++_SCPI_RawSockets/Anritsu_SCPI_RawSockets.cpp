// Cplusplus_generic_SCPI.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include "RawSocketConnection.h"
#include "CustomException.h"

int main()
{	

	RawSocketConnection anritsuSCPIConnection("TCPIP0::127.0.0.1::5001::SOCKET");

	// 0. Instrument connection
	try
	{
		anritsuSCPIConnection.Connect();
	}
	catch (CustomException& exception)
	{
		std::cout << exception.what() << std::endl;
		return -1;
	}

	// 1. Read Instrument type
	std::string idnResponse;
	try
	{
		idnResponse = anritsuSCPIConnection.Query("*IDN?");
	}
	catch (CustomException& exception)
	{
		std::cout << exception.what() << std::endl;
		return -1;
	}
	std::cout << "*IDN? response is: " << idnResponse << std::endl;

	// 2. System Preset
	try
	{
		anritsuSCPIConnection.Send("*RST");
	}
	catch (CustomException& exception)
	{
		std::cout << exception.what() << std::endl;
		return -1;
	}

	// 3. Connection closing
	try
	{
		anritsuSCPIConnection.Disconnect();
	}
	catch (CustomException& exception)
	{
		std::cout << exception.what() << std::endl;
		return -1;
	}
    return 0;
}

