# Anritsu RawSockets MatLab SCPI Commands example solutions

## 1.Overview
Anritsu SCPI RawSockets MatLab Commands are SCPI based solutions which run with Anritsu Instruments.  
This example shows how to perform the following actions in software communication with Anritsu devices:
* Connect
* Write
* Query
* Disconnect (Close connection)

**Programming language and dependency**: MatLab  
**Supported Model(s)**: All Anritsu Instruments  
**Interface(s)**: Ethernet  
**Software version Tested**: MatLab R2017a (9.2.0.556344)  
**Solution Revision**: 1.0  
**Original Release Date**: 6/7/2022  
**Current Revision Date**: 6/7/2022


## 2.Required Software
Some software components need to be installed before using the example solutions. The current versions of these components are listed below, and can be downloaded from their respective website.

[MatLab](https://www.mathworks.com/products/matlab.html)

Refer to the relevant Software Developers resources for more information about software requirements.

## 3.Installation
Refer to the above-mentioned relevant Software Developer resources for more information on required Software installation.
The MatLab Commands example solution require no installation, and can be run as-is.

## 4.Running

>  **Keep in mind that the Anritsu Instrument has to be running *(and be connected to its corresponding software, if it has one)* during the execution of this example solution.**

The example covers the method of sending SCPI commands via a direct Raw TCP Socket connection.

### 4.0. Instrument connection

The instrument connection is instantiated as an object using the address and the port of the device.  
Example address for instrument connection:

    '127.0.0.1', 5001

> **Important Note:**
>> The script example uses the localhost address 127.0.0.1 for a device connected directly to the computer running the script; to work with devices connected to another computer in the network **don't forget to change the instrument address in the first line of the script**.

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
The latest version of this MatLab example, and examples for other programming languages, can be downloaded at [this location](https://github.com/Anritsu/Examples).  

REV 1.1, 2/22/2023  
Modified by: Voicu Bogdan, Bucharest, Romania  
Original Release.