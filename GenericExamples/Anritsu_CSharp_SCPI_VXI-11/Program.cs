using System;

namespace Anritsu_SCPI_VXI_11
{
    class Program
    {
        static void Main(string[] args)
        {
            int resourceTimeout = 20000; // Edit this timeout value according to your needs and prefference.

            string resourceNameVXIConnection = "TCPIP0::127.0.0.1::inst0::INSTR"; // Edit this address to connect via NI's VXI-11 to a VNA device in the network at a different IP address.

            // Object instantiation
            VXI11Connection AnritsuSCPIConnection = new VXI11Connection(resourceNameVXIConnection);

            // 0. Instrument connection
            AnritsuSCPIConnection.Connect(resourceTimeout);

            // 1. Read Instrument type
            string instrumentInfo = AnritsuSCPIConnection.Query("*IDN?");
            Console.WriteLine("*IDN? response is: " + instrumentInfo);

            // 2. System Preset
            AnritsuSCPIConnection.Write("*RST");

            // 3. Connection closing
            AnritsuSCPIConnection.Disconnect();
        }
    }
}
