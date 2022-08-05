# Anritsu VXI-11 Python SCPI Commands example solutions

## 1.Overview
Anritsu SCPI VXI-11 Python Commands are SCPI based solutions which run with Anritsu Instruments.  
This example shows how to perform the following actions in software communication with Anritsu devices:
* Connect
* Write
* Query
* Disconnect (Close connection)

**Programming language and dependency**: Python3 ("PyVISA" Python package)  
**Supported Model(s)**: All Anritsu Instruments  
**Interface(s)**: Ethernet  
**Software version Tested**: Python 3.10.2, PyVISA 1.11.3  
**Solution Revision**: 1.0  
**Original Release Date**: 6/7/2022  
**Current Revision Date**: 6/7/2022


## 2.Required Software
Some software components need to be installed before using the example solutions. The current versions of these components are listed below, and can be downloaded from their respective website.

[Python3](https://www.python.org/downloads/)  
[PyVISA](https://pypi.org/project/PyVISA/) (also note the documentation [here](https://pyvisa.readthedocs.io/en/latest/introduction/getting.html) - this module requires the NI-VISA package)  
[NI-VISA](https://www.ni.com/ro-ro/support/downloads/drivers/download.ni-visa.html)

Refer to the relevant Software Developers resources for more information about software requirements.

## 3.Installation
Refer to the above-mentioned relevant Software Developer resources for more information on required Software installation.  
The Python Commands example solution require no installation, and can be run as-is.

## 4.Running

>  **Keep in mind that the Anritsu Instrument has to be running *(and be connected to its corresponding software, if it has one)* during the execution of this example solution.**

The example covers the method of sending SCPI commands via a VXI-11 connection.

### 4.0. Instrument connection

The instrument connection is instantiated as an object using the resource name of the device.  
Example address for instrument connection:

    TCPIP0::127.0.0.1::inst0::INSTR

> **Important Note:**
>> The script example uses the localhost address 127.0.0.1 for a device connected directly to the computer running the script; to work with devices connected to another computer in the network **don't forget to change the instrument address (and *timeout* value) in the line calling *the main()* function (line 24)**.

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
The latest version of these Python scripts, and examples for other programming languages, can be downloaded at [this location (to be determined)].

REV 1.0, 6/7/2022  
Modified by: Voicu Bogdan, Bucharest, Romania  
Original Release.