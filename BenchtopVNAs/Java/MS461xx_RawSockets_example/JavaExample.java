import java.util.Scanner;

class JavaExample {
    public static void main(String[] args)
    {
        String resourceNameRawSocket = "TCPIP0::127.0.0.1::5001::SOCKET"; // Edit this address to connect to a VNA device in the network at a different IP address.
        int resourceTimeout = 20000; // Edit this timeout value according to your needs and preference.

        // Object instantiation
        RawSocketsConnection rawSocketsConnection = new RawSocketsConnection(resourceNameRawSocket);

        // 0. Instrument connection
        rawSocketsConnection.Connect(resourceTimeout);
        
        // 1. Read Instrument type
        String hardwareIdentity = rawSocketsConnection.Query("*IDN?");
        System.out.println(hardwareIdentity);

        // 2. System preset
        rawSocketsConnection.Write(":SYST:PRES");

        // 3. Define 4 traces - S11, S21, S12, S22
        rawSocketsConnection.Write(":CALC1:PAR:COUN 4");

        rawSocketsConnection.Write(":CALC1:PAR1:DEF S11");
        rawSocketsConnection.Write(":CALC1:PAR2:DEF S21");
        rawSocketsConnection.Write(":CALC1:PAR3:DEF S12");
        rawSocketsConnection.Write(":CALC1:PAR4:DEF S22");

        // 4. Define LOG MAG response
        rawSocketsConnection.Write(":CALC1:PAR1:FORM MLOG");
        rawSocketsConnection.Write(":CALC1:PAR2:FORM MLOG");
        rawSocketsConnection.Write(":CALC1:PAR3:FORM MLOG");
        rawSocketsConnection.Write(":CALC1:PAR4:FORM MLOG");

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
        rawSocketsConnection.Write(":SENS1:CORR:COLL:METH SOLT");
        String correctionCalibrationMethod = rawSocketsConnection.Query(":SENS1:CORR:COLL:METH?");
        System.out.println(String.format("The Calibration method used is: %s.", correctionCalibrationMethod));
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT12:FULL2");
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT1: CONN CFKT");
        String connectorTypePort1 = rawSocketsConnection.Query(":SENS1:CORR:COLL:PORT1:CONN?");
        System.out.println(String.format("Port1 Connector set to %s.", connectorTypePort1));
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT2: CONN CFKT");
        String connectorTypePort2 = rawSocketsConnection.Query(":SENS1:CORR:COLL:PORT2:CONN?");
        System.out.println(String.format("Port2 Connector set to %s.", connectorTypePort2));

        System.out.println("Please connect the LOAD to port 1 and press any key to continue.");
        waitForInput();
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT1:LOAD");
        System.out.println("Please connect the OPEN to port 1 and press any key to continue.");
        waitForInput();
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT1:OPEN");
        System.out.println("Please connect the SHORT to port 1 and press any key to continue.");
        waitForInput();
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT1:SHORT");
        
        System.out.println("Please connect the LOAD to port 2 and press any key to continue.");
        waitForInput();
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT2:LOAD");
        System.out.println("Please connect the OPEN to port 2 and press any key to continue.");
        waitForInput();
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT2:OPEN");
        System.out.println("Please connect the SHORT to port 2 and press any key to continue.");
        waitForInput();
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT2:SHORT");

        System.out.println("Please connect a THRU between port 1 and port 2 and press any key to continue.");
        waitForInput();
        rawSocketsConnection.Write(":SENS1:CORR:COLL:PORT12:THRU");
        rawSocketsConnection.Write(":SENS1:CORR:COLL:SAVE");

        String opcDone1 = rawSocketsConnection.Query("*OPC?"); /* OPC = "Operation Complete Query"; Program flow is allowed to proceed to the next command in the input buffer.*/
        if (!opcDone1.equals("1"))
        {
            System.out.println(opcDone1 + opcDone1);
            System.out.println(opcDone1.getClass().toString());
            System.out.println("1".getClass().toString());
            System.out.println(String.format("Error, Opc returned unexpected value while waiting for calibration to finish (expected '1', recieved %s); ending code execution.", opcDone1));
            rawSocketsConnection.Disconnect();
            System.exit(-1);
        }
        System.out.println("Please connect DUT and press any key to continue.");
        waitForInput();

        // 10. Perform a sweep and wait until done
        rawSocketsConnection.Write(":TRIG:SING");
        String opcDone2 = rawSocketsConnection.Query("*OPC?");        
        if (!opcDone1.equals("1"))
        {
            System.out.println(String.format("Error, Opc returned unexpected value while waiting for single sweep to finish (expected '1', recieved %s); ending code execution.", opcDone2));
            rawSocketsConnection.Disconnect();
            System.exit(-1);
        }

        // 11. Read Frequency List
        System.out.println("Frequency list");
        String frequencyList = rawSocketsConnection.Query(":SENS1:FREQ:DATA?");
        System.out.println(frequencyList);

        // 12. Read S Parameters
        String s11Parameters = rawSocketsConnection.Query(":CALC1:PAR1:DATA:SDAT?");
        String s21Parameters = rawSocketsConnection.Query(":CALC1:PAR2:DATA:SDAT?");
        String s12Parameters = rawSocketsConnection.Query(":CALC1:PAR3:DATA:SDAT?");
        String s22Parameters = rawSocketsConnection.Query(":CALC1:PAR4:DATA:SDAT?");

        System.out.println("Param 1 - S11 - SData");
        System.out.println(s11Parameters);
        System.out.println("Param 2 - S21 - SData");
        System.out.println(s21Parameters);
        System.out.println("Param 3 - S12 - SData");
        System.out.println(s12Parameters);
        System.out.println("Param 4 - S22 - SData");
        System.out.println(s22Parameters);
        
        // 13. Read Formatted Data
        String f11Data = rawSocketsConnection.Query(":CALC1:PAR1:DATA:FDAT?");
        String f21Data = rawSocketsConnection.Query(":CALC1:PAR2:DATA:FDAT?");
        String f12Data = rawSocketsConnection.Query(":CALC1:PAR3:DATA:FDAT?");
        String f22Data = rawSocketsConnection.Query(":CALC1:PAR4:DATA:FDAT?");

        System.out.println("Param 1 - S11 - FData");
        System.out.println(f11Data);
        System.out.println("Param 2 - S21 - FData");
        System.out.println(f21Data);
        System.out.println("Param 3 - S12 - FData");
        System.out.println(f12Data);
        System.out.println("Param 4 - S22 - FData");
        System.out.println(f22Data);

        // 14. Connection closing
        rawSocketsConnection.Disconnect();

        System.out.println();
        System.out.println("Press any key to finish code execution");
        waitForInput();
    }
    private static void waitForInput()
    {
        Scanner scanner = new Scanner(System.in);
        scanner.nextLine();
    }
}