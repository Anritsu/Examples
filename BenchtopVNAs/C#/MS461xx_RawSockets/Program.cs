using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MS461xx_RawSockets_example
{
    class Program
    {
        static void Main(string[] args)
        {
            String resourceNameRawSocket = "TCPIP0::127.0.0.1::5001::SOCKET"; // Edit this address to connect to a VNA device in the network at a different IP address.
            int resourceTimeout = 20000; // Edit this timeout value according to your needs and prefference.

            // Object instantiation
            SCPIConnection rawSocketsConnection = new RawSocketsConnection(resourceNameRawSocket);

            // 0. Instrument connection
            rawSocketsConnection.Connect(resourceTimeout);

            // 1. Read Instrument type
            string instrumentInfo = rawSocketsConnection.Query("*IDN?");
            char instrumentFamily = instrumentInfo[12];
            Console.WriteLine(instrumentInfo);
            Console.WriteLine("Instrument belongs to the {0}00's family", instrumentFamily);

            // 2. System Preset
            rawSocketsConnection.Write(":SYST:PRES");

            // 3. Define 4 traces - S11, S21, S12, S22
            rawSocketsConnection.Write(":CALC1:PAR:COUN 4");

            rawSocketsConnection.Write("CALC1:PAR1:DEF S11");
            rawSocketsConnection.Write("CALC1:PAR2:DEF S21");
            rawSocketsConnection.Write("CALC1:PAR3:DEF S12");
            rawSocketsConnection.Write("CALC1:PAR4:DEF S22");

            // 4. Define LOG MAG response
            rawSocketsConnection.Write("CALC1:PAR1:FORM MLOG");
            rawSocketsConnection.Write("CALC1:PAR2:FORM MLOG");
            rawSocketsConnection.Write("CALC1:PAR3:FORM MLOG");
            rawSocketsConnection.Write("CALC1:PAR4:FORM MLOG");

            // 5. Set frequency range 1GHz - 4GHz
            rawSocketsConnection.Write(":SENS1:FREQ:SPAN 3e9");
            rawSocketsConnection.Write(":SENS1:FREQ:CENT 2.5e9");

            // 6. Set 501 points
            rawSocketsConnection.Write(":SENS1:SWE:POIN 501");

            // 7. Set 100KHz IFBW
            rawSocketsConnection.Write(":SENS1:BAND 1e5");

            // 8. Set instrument on Hold
            rawSocketsConnection.Write(":SENS1:HOLD:FUNC HOLD");

            // 9. Perform a SOLT calibration
            rawSocketsConnection.Write("SENS1:CORR:COLL:METH SOLT");
            string correctionCalibrationMethod = rawSocketsConnection.Query(":SENS1:CORR:COLL:METH?");
            Console.WriteLine("The Calibration method used is: {0}", correctionCalibrationMethod);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT12:FULL2");
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT1:CONN CFKT");
            string connectorTypePort1 = rawSocketsConnection.Query(":SENS1:CORR:COLL:PORT1:CONN?");
            Console.WriteLine("Port1 Connector set to: {0}", connectorTypePort1);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT2:CONN CFKT");
            string connectorTypePort2 = rawSocketsConnection.Query(":SENS1:CORR:COLL:PORT2:CONN?");
            Console.WriteLine("Port2 Connector set to: {0}", connectorTypePort2);
            Console.WriteLine("Please connect the LOAD to port 1 and press any key to continue");
            Console.ReadKey(true);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT1:LOAD");
            Console.WriteLine("Please connect the OPEN to port 1 and press any key to continue");
            Console.ReadKey(true);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT1:OPEN");
            Console.WriteLine("Please connect the SHORT to port 1 and press any key to continue");
            Console.ReadKey(true);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT1:SHORT");
            Console.WriteLine("Please connect the LOAD to port 2 and press any key to continue");
            Console.ReadKey(true);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT2:LOAD");
            Console.WriteLine("Please connect the OPEN to port 2 and press any key to continue");
            Console.ReadKey(true);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT2:OPEN");
            Console.WriteLine("Please connect the SHORT to port 2 and press any key to continue");
            Console.ReadKey(true);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT2:SHORT");
            Console.WriteLine("Please connect a THRU between port 1 and port 2 and press any key to continue");
            Console.ReadKey(true);
            rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT12:THRU");
            rawSocketsConnection.Write(":SENS1:CORR:COLL:SAVE");
            string opcDone1 = rawSocketsConnection.Query("*OPC?"); /* OPC = "Operation Complete Query"; Program flow is allowed to proceed to the next command in the input buffer.*/
            if (opcDone1 != "1")
            {
                Console.WriteLine("Error, Opc returned unexpected value while waiting for calibration to finish (expected '1', received {0}); ending code execution.", opcDone1);
                rawSocketsConnection.Disconnect();
                Environment.Exit(0);
            }
            Console.WriteLine("Please connect DUT and press any key to continue");
            Console.ReadKey(true);

            // 10. Perform a sweep and wait until done
            rawSocketsConnection.Write(":TRIG:SING");
            string opcDone2 = rawSocketsConnection.Query("*OPC?"); //OPC = "Operation Complete Query"; Program flow is allowed to proceed to the next command in the input buffer.
            if (opcDone2 != "1")
            {
                Console.WriteLine("Error, Opc returned unexpected value while waiting for calibration to finish (expected '1', received {0}); ending code execution.", opcDone2);
                rawSocketsConnection.Disconnect();
                Environment.Exit(0);
            }

            // 11. Read Frequency list
            Console.WriteLine("Frequency list");
            string frequencyList = rawSocketsConnection.Query("SENS1:FREQ:DATA?");
            Console.WriteLine(frequencyList);

            // 12. Read S Parameters
            string s11Parameters = rawSocketsConnection.Query(":CALC1:PAR1:DATA:SDAT?");
            string s21Parameters = rawSocketsConnection.Query(":CALC1:PAR2:DATA:SDAT?");
            string s12Parameters = rawSocketsConnection.Query(":CALC1:PAR3:DATA:SDAT?");
            string s22Parameters = rawSocketsConnection.Query(":CALC1:PAR4:DATA:SDAT?");

            Console.WriteLine("Param 1 - S11 - SData");
            Console.WriteLine(s11Parameters);
            Console.WriteLine("Param 2 - S21 - SData");
            Console.WriteLine(s21Parameters);
            Console.WriteLine("Param 3 - S12 - SData");
            Console.WriteLine(s12Parameters);
            Console.WriteLine("Param 4 - S22 - SData");
            Console.WriteLine(s22Parameters);

            // 13. Read Formatted Data
            string f11Data = rawSocketsConnection.Query(":CALC1:PAR1:DATA:FDAT?");
            string f21Data = rawSocketsConnection.Query(":CALC1:PAR2:DATA:FDAT?");
            string f12Data = rawSocketsConnection.Query(":CALC1:PAR3:DATA:FDAT?");
            string f22Data = rawSocketsConnection.Query(":CALC1:PAR4:DATA:FDAT?");

            Console.WriteLine("Param 1 - S11 - FData");
            Console.WriteLine(f11Data);
            Console.WriteLine("Param 2 - S12 - FData");
            Console.WriteLine(f21Data);
            Console.WriteLine("Param 3 - S21 - FData");
            Console.WriteLine(f12Data);
            Console.WriteLine("Param 4 - S22 - FData");
            Console.WriteLine(f22Data);

            // 14. Connection closing
            rawSocketsConnection.Disconnect();

            Console.WriteLine("\nPress any key to finish code execution");
            Console.ReadKey(true);
        }
    }
}
