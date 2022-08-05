// Anritsu_SCPI_VXI11.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include "VXI11Connection.h"
#include "CustomException.h"


int main()
{	 
	// Object instantiation
	VXI11Connection anritsuSCPIConnection("TCPIP0::127.0.0.1::inst0::INSTR");

	// 0. Instrument connection
	try
	{
		anritsuSCPIConnection.Connect();
	}
	catch (CustomException& exception)
	{
		std::cout << exception.toString() << std::endl;
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
		std::cout << exception.toString() << std::endl;
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
		std::cout << exception.toString() << std::endl;
		return -1;
	}

	// 3. Connection closing
	anritsuSCPIConnection.Disconnect();
    return 0;
}

