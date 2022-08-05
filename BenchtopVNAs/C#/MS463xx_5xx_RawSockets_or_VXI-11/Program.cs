using System;

namespace MS463xx_5xx_RawSockets_or_VXI_11_ex
{
    class Program
    {
        static void Main(string[] args)
        {
            int resourceTimeout = 20000; // Edit this timeout value according to your needs and prefference.

            //string resourceNameRawSocket = "TCPIP0::127.0.0.1::5001::SOCKET"; // Edit this address to connect via Sockets to a VNA device in the network at a different IP address.
            string resourceNameVXIConnection = "TCPIP0::127.0.0.1::inst0::INSTR"; // Edit this address to connect via NI's VXI-11 to a VNA device in the network at a different IP address.

            // Object instantiation
            //SCPIConnection rawSocketConnection = new RawSocketsConnection(resourceNameRawSocket);
            SCPIConnection visaConnection = new NIVXI11Connection(resourceNameVXIConnection);

            //RunExample(rawSocketConnection, resourceTimeout);
            RunExample(visaConnection, resourceTimeout);

        }
        static void RunExample(SCPIConnection connection, int timeout)
        {
            // 0. Instrument connection
            connection.Connect(timeout);

            // 1. Read Instrument type
            string instrumentInfo = connection.Query("*IDN?");
            char instrumentFamily = instrumentInfo[12];
            Console.WriteLine(instrumentInfo);
            Console.WriteLine("Instrument belongs to the {0}00's family", instrumentFamily);

            // 2. System Preset
            connection.Write(":SYST:PRES");

            // 3. Define 4 traces - S11, S21, S12, S22
            connection.Write(":CALC1:PAR:COUN 4");

            connection.Write("CALC1:PAR1:DEF S11");
            connection.Write("CALC1:PAR2:DEF S21");
            connection.Write("CALC1:PAR3:DEF S12");
            connection.Write("CALC1:PAR4:DEF S22");

            // 4. Define LOG MAG response
            connection.Write("CALC1:PAR1:FORM MLOG");
            connection.Write("CALC1:PAR2:FORM MLOG");
            connection.Write("CALC1:PAR3:FORM MLOG");
            connection.Write("CALC1:PAR4:FORM MLOG");

            // 5. Set frequency range 1GHz - 4GHz
            connection.Write(":SENS1:FREQ:SPAN 3e9");
            connection.Write(":SENS1:FREQ:CENT 2.5e9");

            // 6. Set 501 points
            connection.Write(":SENS1:SWE:POIN 501");

            // 7. Set 100KHz IFBW
            connection.Write(":SENS1:BAND 1e5");

            // 8. Set instrument on Hold
            connection.Write(":SENS1:HOLD:FUNC HOLD");

            // 9. Perform a SOLT calibration
            connection.Write("SENS1:CORR:COLL:METH SOLT");
            string correctionCalibrationMethod = connection.Query(":SENS1:CORR:COLL:METH?");
            Console.WriteLine("The Calibration method used is: {0}", correctionCalibrationMethod);
            connection.Write(":SENS1:CORR:COLL:PORT12:FULL2");
            connection.Write(":SENS1:CORR:COLL:PORT1:CONN CFKT");
            string connectorTypePort1 = connection.Query(":SENS1:CORR:COLL:PORT1:CONN?");
            Console.WriteLine("Port1 Connector set to: {0}", connectorTypePort1);
            connection.Write(":SENS1:CORR:COLL:PORT2:CONN CFKT");
            string connectorTypePort2 = connection.Query(":SENS1:CORR:COLL:PORT2:CONN?");
            Console.WriteLine("Port2 Connector set to: {0}", connectorTypePort2);
            Console.WriteLine("Please connect the LOAD to port 1 and press any key to continue");
            Console.ReadKey(true);
            connection.Write(":SENS1:CORR:COLL:PORT1:LOAD");
            Console.WriteLine("Please connect the OPEN to port 1 and press any key to continue");
            Console.ReadKey(true);
            connection.Write(":SENS1:CORR:COLL:PORT1:OPEN");
            Console.WriteLine("Please connect the SHORT to port 1 and press any key to continue");
            Console.ReadKey(true);
            connection.Write(":SENS1:CORR:COLL:PORT1:SHORT");
            Console.WriteLine("Please connect the LOAD to port 2 and press any key to continue");
            Console.ReadKey(true);
            connection.Write(":SENS1:CORR:COLL:PORT2:LOAD");
            Console.WriteLine("Please connect the OPEN to port 2 and press any key to continue");
            Console.ReadKey(true);
            connection.Write(":SENS1:CORR:COLL:PORT2:OPEN");
            Console.WriteLine("Please connect the SHORT to port 2 and press any key to continue");
            Console.ReadKey(true);
            connection.Write(":SENS1:CORR:COLL:PORT2:SHORT");
            Console.WriteLine("Please connect a THRU between port 1 and port 2 and press any key to continue");
            Console.ReadKey(true);
            connection.Write(":SENS1:CORR:COLL:PORT12:THRU");
            connection.Write(":SENS1:CORR:COLL:SAVE");
            string opcDone1 = connection.Query("*OPC?"); /* OPC = "Operation Complete Query"; Program flow is allowed to proceed to the next command in the input buffer.*/
            if (opcDone1 != "1")
            {
                Console.WriteLine("Error, Opc returned unexpected value while waiting for calibration to finish (expected '1', received {0}); ending code execution.", opcDone1);
                connection.Disconnect();
                Environment.Exit(0);
            }
            Console.WriteLine("Please connect DUT and press any key to continue");
            Console.ReadKey(true);

            // 10. Perform a sweep and wait until done
            connection.Write(":TRIG:SING");
            string opcDone2 = connection.Query("*OPC?"); //OPC = "Operation Complete Query"; Program flow is allowed to proceed to the next command in the input buffer.
            if (opcDone2 != "1")
            {
                Console.WriteLine("Error, Opc returned unexpected value while waiting for calibration to finish (expected '1', received {0}); ending code execution.", opcDone2);
                connection.Disconnect();
                Environment.Exit(0);
            }

            // 11. Read Frequency list
            Console.WriteLine("Frequency list");
            string frequencyList = connection.Query("SENS1:FREQ:DATA?");
            Console.WriteLine(frequencyList);

            // 12. Read S Parameters
            string s11Parameters = connection.Query(":CALC1:PAR1:DATA:SDAT?");
            string s21Parameters = connection.Query(":CALC1:PAR2:DATA:SDAT?");
            string s12Parameters = connection.Query(":CALC1:PAR3:DATA:SDAT?");
            string s22Parameters = connection.Query(":CALC1:PAR4:DATA:SDAT?");

            Console.WriteLine("Param 1 - S11 - SData");
            Console.WriteLine(s11Parameters);
            Console.WriteLine("Param 2 - S21 - SData");
            Console.WriteLine(s21Parameters);
            Console.WriteLine("Param 3 - S12 - SData");
            Console.WriteLine(s12Parameters);
            Console.WriteLine("Param 4 - S22 - SData");
            Console.WriteLine(s22Parameters);

            // 13. Read Formatted Data
            string f11Data = connection.Query(":CALC1:PAR1:DATA:FDAT?");
            string f21Data = connection.Query(":CALC1:PAR2:DATA:FDAT?");
            string f12Data = connection.Query(":CALC1:PAR3:DATA:FDAT?");
            string f22Data = connection.Query(":CALC1:PAR4:DATA:FDAT?");

            Console.WriteLine("Param 1 - S11 - FData");
            Console.WriteLine(f11Data);
            Console.WriteLine("Param 2 - S12 - FData");
            Console.WriteLine(f21Data);
            Console.WriteLine("Param 3 - S21 - FData");
            Console.WriteLine(f12Data);
            Console.WriteLine("Param 4 - S22 - FData");
            Console.WriteLine(f22Data);

            // 14. Connection closing
            connection.Disconnect();

            Console.WriteLine("\nPress any key to finish code execution");
            Console.ReadKey(true);
        }
    }
}
