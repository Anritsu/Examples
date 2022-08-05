using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Anritsu_SCPI_RawSockets
{ 
    class Program
    {
        static void Main(string[] args)
        {
            String resourceNameRawSocket = "TCPIP0::127.0.0.1::5001::SOCKET"; // Edit this address to connect to a VNA device in the network at a different IP address.
            int resourceTimeout = 20000; // Edit this timeout value according to your needs and prefference.

            // Object instantiation
            RawSocketsConnection AnritsuSCPIConnection = new RawSocketsConnection(resourceNameRawSocket);

            // 0. Instrument connection
            AnritsuSCPIConnection.Connect(resourceTimeout);

            // 1. Read Instrument type
            string instrumentInfo = AnritsuSCPIConnection.Query("*IDN?");
            Console.WriteLine(instrumentInfo);

            // 2. System Preset
            AnritsuSCPIConnection.Write("*RST");

            // 3. Connection closing
            AnritsuSCPIConnection.Disconnect();

        }
    }
}
