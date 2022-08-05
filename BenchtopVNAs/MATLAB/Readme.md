# Anritsu ShockLine VNA MS46xxx Series MATLAB Commands example scripts

## 1.Overview
Anritsu ShockLine VNA MS46XXX Series MATLAB Commands are SCPI based MATLAB scripts which run with Anritsu ShockLine VNAs.

**Programming language and dependency**: MATLAB (+ Instrument Control Toolbox) \
**Supported Model(s)**: MS46121A, MS46121B, MS46122A, MS46122B, MS46131A, MS46322A, MS46322B, MS46522A, MS46522B, MS46524A, MS46524B, ME7868A \
**Model(s) Tested**: MS46122B, MS46522B \
**Interface(s)**: Ethernet \
**Software version Tested**: MATLAB R2017a (9.2.0.556344) 64-bit, Instrument Control Toolbox V3.11 (R2017a) \
**VNA Software Revision(s) Tested**: V2020.7.1, V2021.5.2

**Script Revision**: 1.0 \
**Original Release Date**: 7/27/2022 \
**Current Revision Date**: 7/27/2022

## 2.Required Software
Some software components need to be installed before using these scripts. The current versions of these components are listed below, and can be downloaded from their respective website.

[MATLAB](https://www.mathworks.com/products/matlab.html) \
[Instrument Control Toolbox](https://www.mathworks.com/products/instrument.html)

Refer to the relevant Software Developers resources for more information about software requirements.

## 3.Installation
Refer to the above-mentioned relevant Software Developer resources for more information on required Software installation. \
The MATLAB Commands example scripts require no installation, and can be run as-is.

## 4.Running

>  **Keep in mind that the ShockLine software has to be running during the execution of this script.**

The script examples cover two main methods of sending SCPI commands:

    * Via a direct Socket connection
    * Via TCP/IP with National Instrument's NI-Visa support (VXI-11)

Not all VNA devices support VXI-11 connections (using the Instrument Control Toolbox):

    * If your device belongs to the 100's family, please refer to the example script: "MS461xx_RawSockets_example.m".
    * If your device belongs to the 300's  or 500's family, please refer to the example script: "MS463xx_5xx_RawSockets_and_VXI11.m".

Both examples, regardless of connection method, function in a similar manner, described bellow:
* Note: The example script contains a SOLT calibration (requiring a VNA with a minimum of 2 ports). For a single-port VNA, skip the calibration step to have a good functional example.
* Note: For further understanding of individual SCPI commands, please consult the ShockLine Series VNA [Programming Manual](https://dl.cdn-anritsu.com/en-us/test-measurement/files/Manuals/Programming-Manual/10410-00746Y.pdf)
* Note: All configurations of the examples are performed on a single channel - "CALC{1}" / "SENS{1}". Users can however use as many as 16 different channels, each with individual configuration

### 4.0. Instrument connection

The instrument connection is instantiated as a class object depending on the connection type. \
Example addresses for instrument connection: 

    * TCPIP0::127.0.0.1::5001::SOCKET
    * TCPIP0::127.0.0.1::inst0::INSTR ( only for 300's and 500's)

> **Important Note:**
>> The script examples use the localhost address 127.0.0.1 for a device connected directly to the computer running the script; __don't forget to change the instrument address in the variable "resource_name" in the first line of the script__ for devices connected to another computer in the network.

In the example "MS463xx_5xx_RawSockets_and_VXI11.m", the connection method is determined by the last parameter of this address ("SOCKET" or "INSTR").

Manipulating the address string is handled by classes defined for instrument connection.

**The timeout value should be carefully considered**. For example, step 10 in these example scripts can fail for the reason that the "waiting" operation takes longer than the permitted "timeout" input from the user.

If the connection fails for any reason, the error message is displayed in the terminal, and the script stops running completely.

### 4.1. Read Instrument type
Simple example of a query command; \
Returns information about the device manufacturer, model, serial, and firmware revision number.

### 4.2. System preset
Simple example of a write command; \
The instrument is returned to its factory as-shipped configuration.

### 4.3. Set and define traces
First, send one SCPI write command to define a number of traces (4 in the example scripts; acceptable range = 1 - 16). \
Each trace is then individually referred to in the ":PAR{1-16}" part of the next 4 SCPI commands for defining(/setting the measurement parameters - see programming manual).

### 4.4. Trace response formatting
Similarly, the response is then formatted for each of the traces. In the example, all traces are set to "Log Magnitude" format.

### 4.5. Set frequency range
The frequency range can be set with 2 SCPI write commands. In this case the "SPAN" and "CENTer" commands; \
Alternatively, the range can be set STARt and STOP write commands, for example, the SCPI commands:

* instrument.write(":SENS1:FREQ:STAR 1e9")
* instrument.write(":SENS1:FREQ:CTOP 4e9")

would result in the same outcome as the example script, a frequency range spanning 3 GHz, with the center of the range at 2.5 GHz.

### 4.6. Set number of points
Simple SCPI write command, sets the number of measurement points for the sweep.

### 4.7. Set Intermediate frequency Bandwidth
Simple SCPI write command, the command in the example functions exactly the same as ":SENS1:BWID".

### 4.8. Set instrument Hold function
Example script sets the hold function of the instrument to "HOLD" / alternative settings are "CONTinuous" and "SINGle".

### 4.9. Perform a calibration
Example script sets the calibration method to "Short-Open-Load-Through". \
Refer to the programming manual for all available options.

### 4.10. Perform a single sweep and wait.
Example script triggers a Single Sweep of the device. \
The "*OPC?" (Operation Complete) query is a special query. While it always returns the character "1", the example script does not display it. \
It's used to "wait" and block commands in input buffer until the end of the current operation. \
In this case, the single sweep set right before it. \
An un-printed query is used instead of the "*OPC" write command because they are not a command/query pair - see programming manual.

### 4.11. Read Frequency list
Simple SCPI query, outputs the Frequency list (the spread of the 501 measurement points set earlier along the 1GHz - 4GHz frequency range set earlier).

### 4.12. Read S parameters
Outputs, for each trace, the raw S-parameter values at each of the measurement points.

### 4.13. Read Formatted data
Outputs, for each trace, the formatted data values at each of the measurement points. (Formatting type was set at step 4).

### 4.14. Instrument connection closing.
The connection object will end the relevant connection method depending on the type it instantiated at step 0.

## 5.Revision History
The latest version of these MATLAB scripts, and examples for other programming languages, can be downloaded at [this location (to be determined)]. \
REV 1.0, 07/27/2022 \
Modified by: Voicu Bogdan, Bucharest, Romania \
Original Release. 