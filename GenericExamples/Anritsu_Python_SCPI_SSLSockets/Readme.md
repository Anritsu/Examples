# Anritsu Secure Sockets Python SCPI Commands example solutions

## 1.Overview
Anritsu SCPI SSL Sockets Python Commands are SCPI based solutions which run with Anritsu Instruments.  
This example shows how to perform the following actions in software communication with Anritsu devices which have Secure Communication Option(0017) enabled:
* Connect
* Write
* Query
* Disconnect (Close connection)

**Programming language and dependency**: Python3 ("socket","ssl", Python modules)  
**Supported Model(s)**: FieldMaster[MS2070A,MS2080A] ,FieldMaster Pro[MS2090A], SiteMaster[MS2085A, MS2089A], Spectrum Monitor [MS2720xA]
**Interface(s)**: Ethernet  


## 2.Required Software
Some software components need to be installed before using the example solutions. The current versions of these components are listed below, and can be downloaded from their respective website.

[Python3](https://www.python.org/downloads/)

Refer to the relevant Software Developers resources for more information about software requirements.

## 3.Installation
Refer to the above-mentioned relevant Software Developer resources for more information on required Software installation.  
The Python Commands example solution require no installation, and can be run as-is.

## 4.Running

>  **Keep in mind that the Anritsu Instrument has to be running *(and be connected to its corresponding software, if it has one)* during the execution of this example solution.**

The example covers the method of sending SCPI commands via a direct SSL Socket connection.

### 4.0. Instrument connection

The instrument connection is instantiated as an object using the resource name of the device.  
Example address for instrument connection:

    TCPIP0::127.0.0.1::9003::SOCKET

    With Secure Communication the TCP socket 9003 is encrypted. The connection can be opended using the SSL module

```python
ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
ssl_context.check_hostname = False
ssl_context.verify_mode = ssl.CERT_NONE
raw_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)self.ssl_socket = ssl_context.wrap_socket(raw_socket, server_hostname=self.host_ip)
self.ssl_socket.settimeout(timeout)
self.connect()
```


> **Important Note:**
>> The script example uses the localhost address 127.0.0.1 for a device connected directly to the computer running the script; to work with devices connected to another computer in the network **don't forget to change the instrument address (and *timeout* value) in the line calling *the main()* function**.

If the connection fails for any reason, the error message is displayed in the terminal, and the script stops running completely.

### 4.1. Read Instrument type - *IDN? Query
Simple example of a query command;  
Returns information about the device manufacturer, model, serial, and firmware revision number.

### 4.2. Upload a file to instrument.
Example of uploading a file to the Instrument.


### 4.3 Download a file from the instrument.
Example of downloading a file from the Instrument.

### 4.4. Instrument connection closing
The connection object will end the connection executed at step **4.0**.
