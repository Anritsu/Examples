#include <iostream>
#include "VXIConnection.h"
#include "RawSocketConnection.h"
#include "CustomException.h"

void SCPI_example(SCPIConnection* instrument, int timeout) {

    // 0.Instrument connection
    instrument->Connect();
    instrument->SetTimeout(timeout);

    // 1. Read Instrument type
    std::string instrumentInfo = instrument->Query("*IDN?");
    char instrumentFamily = instrumentInfo[12];
    std::cout << instrumentInfo << std::endl;
    std::cout << "Instrument belongs to the " << instrumentFamily << "00's family" << std::endl;

    // 2. System Preset
    instrument->Write(":SYST:PRES");

    // 3. Define 4 traces - S11, S21, S12, S22
    instrument->Write(":CALC1:PAR:COUN 4");

    instrument->Write("CALC1:PAR1:DEF S11");
    instrument->Write("CALC1:PAR2:DEF S21");
    instrument->Write("CALC1:PAR3:DEF S12");
    instrument->Write("CALC1:PAR4:DEF S22");

    // 4. Define LOG MAG response
    instrument->Write("CALC1:PAR1:FORM MLOG");
    instrument->Write("CALC1:PAR2:FORM MLOG");
    instrument->Write("CALC1:PAR3:FORM MLOG");
    instrument->Write("CALC1:PAR4:FORM MLOG");

    // 5. Set frequency range 1GHz - 4GHz
    instrument->Write(":SENS1:FREQ:SPAN 3e9");
    instrument->Write(":SENS1:FREQ:CENT 2.5e9");

    // 6. Set 501 points
    instrument->Write(":SENS1:SWE:POIN 501");

    // 7. Set 100KHz IFBW
    instrument->Write(":SENS1:BAND 1e5");

    // 8. Set instrument on Hold
    instrument->Write(":SENS1:HOLD:FUNC HOLD");

    // 9. Perform a SOLT calibration
    instrument->Write("SENS1:CORR:COLL:METH SOLT");
    std::string correctionCalibrationMethod = instrument->Query(":SENS1:CORR:COLL:METH?");
    std::cout << "The Calibration method used is: " << correctionCalibrationMethod << "." << std::endl;
    instrument->Write(":SENS1:CORR:COLL:PORT12:FULL2");
    instrument->Write(":SENS1:CORR:COLL:PORT1:CONN CFKT");
    std::string connectorTypePort1 = instrument->Query(":SENS1:CORR:COLL:PORT1:CONN?");
    std::cout << "Port1 Connector set to: " << connectorTypePort1 << "." << std::endl;
    std::string connectorTypePort2 = instrument->Query(":SENS1:CORR:COLL:PORT2:CONN?");
    std::cout << "Port2 Connector set to: " << connectorTypePort1 << "." << std::endl;
    std::cout << "Please connect the LOAD to port 1 and press Enter to continue." << std::endl;
    std::cin.get();
    instrument->Write(":SENS1:CORR:COLL:PORT1:LOAD");
    std::cout << "Please connect the OPEN to port 1 and press Enter to continue." << std::endl;
    std::cin.get();
    instrument->Write(":SENS1:CORR:COLL:PORT1:OPEN");
    std::cout << "Please connect the SHORT to port 1 and press Enter to continue." << std::endl;
    std::cin.get();
    instrument->Write(":SENS1:CORR:COLL:PORT1:SHORT");
    std::cout << "Please connect the LOAD to port 2 and press Enter to continue." << std::endl;
    std::cin.get();
    instrument->Write(":SENS1:CORR:COLL:PORT2:LOAD");
    std::cout << "Please connect the OPEN to port 2 and press Enter to continue." << std::endl;
    std::cin.get();
    instrument->Write(":SENS1:CORR:COLL:PORT2:OPEN");
    std::cout << "Please connect the SHORT to port 2 and press Enter to continue." << std::endl;
    std::cin.get();
    instrument->Write(":SENS1:CORR:COLL:PORT2:SHORT");
    std::cout << "Please connect a THRU between port 1 and port 2 and press Enter to continue." << std::endl;
    std::cin.get();
    instrument->Write(":SENS1:CORR:COLL:PORT12:THRU");
    instrument->Write(":SENS1:CORR:COLL:SAVE");
    std::string opcDone1 = instrument->Query("*OPC?"); // OPC = "Operation Complete Query"; Program flow is allowed to proceed to the next command in the input buffer.
    if (opcDone1 != "1")
    {
        throw CustomException("OPC returned an unexpected value while waiting for calibration to finish - expected \"1\", received: " + opcDone1 + ".");
    }

    std::cout << "Please connect DUT and press Enter to continue." << std::endl;
    std::cin.get();

    // 10. Perform a sweep and wait until done
    instrument->Write(":TRIG:SING");
    std::string opcDone2 = instrument->Query("*OPC?");
    if (opcDone1 != "1")
    {
        throw CustomException("OPC returned an unexpected value while waiting for calibration to finish - expected \"1\", received: " + opcDone2 + ".");
    }

    // 11. Read Frequency list
    std::cout << "Frequency list:" << std::endl;
    std::string frequencyList = instrument->Query("SENS1:FREQ:DATA?");
    std::cout << frequencyList << std::endl;

    // 12. Read S Parameters
    std::string s11Parameters = instrument->Query(":CALC1:PAR1:DATA:SDAT?");
    std::string s21Parameters = instrument->Query(":CALC1:PAR2:DATA:SDAT?");
    std::string s12Parameters = instrument->Query(":CALC1:PAR3:DATA:SDAT?");
    std::string s22Parameters = instrument->Query(":CALC1:PAR4:DATA:SDAT?");

    std::cout << "Param 1 - S11 - SData" << std::endl;
    std::cout << s11Parameters << std::endl;
    std::cout << "Param 2 - S21 - SData" << std::endl;
    std::cout << s21Parameters << std::endl;
    std::cout << "Param 3 - S12 - SData" << std::endl;
    std::cout << s12Parameters << std::endl;
    std::cout << "Param 4 - S22 - SData" << std::endl;
    std::cout << s22Parameters << std::endl;

    // 13. Read Formatted Data
    std::string f11Data = instrument->Query(":CALC1:PAR1:DATA:FDAT?");
    std::string f21Data = instrument->Query(":CALC1:PAR2:DATA:FDAT?");
    std::string f12Data = instrument->Query(":CALC1:PAR3:DATA:FDAT?");
    std::string f22Data = instrument->Query(":CALC1:PAR4:DATA:FDAT?");

    std::cout << "Param 1 - S11 - FData" << std::endl;
    std::cout << f11Data << std::endl;
    std::cout << "Param 2 - S21 - FData" << std::endl;
    std::cout << f21Data << std::endl;
    std::cout << "Param 3 - S12 - FData" << std::endl;
    std::cout << f12Data << std::endl;
    std::cout << "Param 4 - S22 - FData" << std::endl;
    std::cout << f22Data << std::endl;

    // EXTRA: Timeout error handling example

    std::cout << std::endl;
    instrument->SetTimeout(1); //setting timeout to lowest possible time to guarantee timeout on any time-consuming operation
    try
    {
        instrument->Write(":TRIG:SING");
        std::string timeoutOPC = instrument->Query("*OPC?");
        if (timeoutOPC != "1")
        {
            throw CustomException("OPC returned an unexpected value while waiting for calibration to finish - expected \"1\", received: " + timeoutOPC + ".");
        }
    }
    catch (SocketCommunicationTimeoutError& scte)
    {
        // When a timeout error occurs, the device will queue a response to a SCPI command, but will never deliver it because of the error.
        // We therefore 'flush' this response by setting the timeout value higher, and then reading the response of a 'dummy' query. 
        // The dummy query 'command' can be an empty string.
        // If it's a valid query, the value recieved is likely compromised anyway, AND it might queue more actions that may lead to timeout errors.

        std::cout << "Error '" << scte.what() << "' caught" << std::endl;
        instrument->SetTimeout(50000);  //Timeout needs to first be set to a sufficiently high value
                                        //If we don't do this, than whatever operation caused the timeout error will only have twice the initial timeout time to complete (2ms in this example)

        std::string dummyValue = instrument->Query("*IDN?"); //Then we flush the the first queued response.

        std::cout << "Compromised dummy value = " << dummyValue << std::endl; // Display the value, very likely to be a compromised value.


        std::string timeoutOPC = instrument->Query("*OPC?"); //Retry the original failed query command
        if (timeoutOPC != "1")
        {
            throw CustomException("OPC returned an unexpected value while waiting for calibration to finish - expected \"1\", received: " + timeoutOPC + ".");
        }

        if (timeoutOPC == "1")
        {
            std::cout << "Succesfully handeled the timeout error" << std::endl;
        }
    }
    catch (VisaCommunicationTimeoutError& vcte)
    {
        // When a timeout error occurs, the device will queue a response to a SCPI command, but will never deliver it because of the error.
        // We therefore 'flush' this response by setting the timeout value higher, and then reading the response of a 'dummy' query. 
        // The dummy query 'command' can be an empty string.
        // If it's a valid query, the value recieved is likely compromised anyway, AND it might queue more actions that may lead to timeout errors.

        std::cout << "Error '" << vcte.what() << "' caught" << std::endl;
        instrument->SetTimeout(50000);  //Timeout needs to first be set to a sufficiently high value
                                        //If we don't do this, than whatever operation caused the timeout error will only have twice the initial timeout time to complete (2ms in this example)

        std::string dummyValue = instrument->Query("*IDN?"); //Then we flush the the first queued response.

        std::cout << "Compromised dummy value = " << dummyValue << std::endl; // Display the value, very likely to be a compromised value.


        std::string timeoutOPC = instrument->Query("*OPC?"); //Retry the original failed query command
        if (timeoutOPC != "1")
        {
            throw CustomException("OPC returned an unexpected value while waiting for calibration to finish - expected \"1\", received: " + timeoutOPC + ".");
        }

        if (timeoutOPC == "1")
        {
            std::cout << "Succesfully handeled the timeout error" << std::endl;
        }

    }

    instrument->SetTimeout(timeout);

    // 14. Connection closing
    instrument->Disconnect();

}

int main()
{
    //RawSocket Connection example:

    std::string resourceNameRawSocket = "TCPIP0::127.0.0.1::5001::SOCKET";
    int resourceTimeoutRawSocket = 20000;
        
    RawSocketConnection rawSocketsConnection(resourceNameRawSocket);
        
    SCPI_example(&rawSocketsConnection, resourceTimeoutRawSocket);

    std::cout << "\n\nFinished RawSocket Connection example \n\n" << std::endl;


    //VXI-11 Connection example:

    std::string resourceNameVXIConnection = "TCPIP0::127.0.0.1::inst0::INSTR";
    int resourceTimeoutVXIConnection = 20000;

    VXIConnection VXIConnection(resourceNameVXIConnection);

    SCPI_example(&VXIConnection, resourceTimeoutVXIConnection);

    std::cout << "\n\nFinished VXI-11 Connection example \n\n" << std::endl;

    std::cout << "\nPress Enter to finish code execution" << std::endl;
    std::cin.get();
}
