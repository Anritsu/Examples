# Anritsu RawSockets C++ SCPI Commands example solutions

## 1.Overview
Anritsu SCPI RawSockets C++ Commands are SCPI based solutions which run with Anritsu Instruments.  
This example shows how to perform the following actions in software communication with Anritsu devices:
* Connect
* Write
* Query
* Disconnect (Close connection)

**Programming language and dependency**: C++, Microsoft Visual Studio 2015 Platform Toolset - v140, Target Platform Version - 8.1  
**Supported Model(s)**: All Anritsu Instruments  
**Interface(s)**: Ethernet  
**Software version Tested**: MVS2019 V16.11.11 ; MVS2015 V14.0.23107.0, Visual Studio - SDK 10.0.10586;  
**Solution Revision**: 1.0  
**Original Release Date**: 6/7/2022  
**Current Revision Date**: 6/7/2022


## 2.Required Software
Some software components need to be installed before using the example solutions. The current versions of these components are listed below, and can be downloaded from their respective website.

[Visual Studio](https://visualstudio.microsoft.com/downloads/)

Refer to the relevant Software Developers resources for more information about software requirements.

## 3.Installation
Refer to the above-mentioned relevant Software Developer resources for more information on required Software installation.  
The C++ Commands example solution require no installation, and can be opened as a project in Microsoft Visual Studio as-is, to be run, edited and/or built into an executable.

## 4.Running

>  **Keep in mind that the Anritsu Instrument has to be running *(and be connected to its corresponding software, if it has one)* during the execution of this example solution.**

The example covers the method of sending SCPI commands via a direct Raw TCP Socket connection.

### 4.0. Instrument connection

The instrument connection is instantiated as an object using the resource name of the device.  
Example address for instrument connection:

    TCPIP0::127.0.0.1::5001::SOCKET

> **Important Note:**
>> The script example uses the localhost address 127.0.0.1 for a device connected directly to the computer running the script; to work with devices connected to another computer in the network **don't forget to change the instrument address in the first line of the *main()* function**.

If the connection fails for any reason, the error message is displayed in the terminal, and the script stops running completely.

### 4.1. Read Instrument type - *IDN? Query
Simple example of a query command;  
Returns information about the device manufacturer, model, serial, and firmware revision number.

### 4.2. System preset - *RST
Simple example of a write command;  
The instrument is returned to its factory as-shipped configuration.

### 4.3. Instrument connection closing
The connection object will end the connection executed at step **4.0**.

## 5.Revision History
The latest version of this C++ example, and examples for other programming languages, can be downloaded at [this location (to be determined)].

REV 1.0, 6/7/2022  
Modified by: Voicu Bogdan, Bucharest, Romania  
Original Release.