using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace STEP
{
    class Program
    {
        static SerialPort serialPort;

        static void Main(string[] args)
        {
            Console.WriteLine("Start STEP - Serial - SCPI\n");

            // Assumptions:
            // 1. 

            try
            {
                // Open the serial port.
                serialPort = new SerialPort();
                OpenSerialPort(serialPort);

                // Set SCPI language
                WriteSerialPort(serialPort, "SYST:LANG SCPI\n");

                // Send *IDN?
                WriteSerialPort(serialPort, "*IDN?\n");

                // Read the response.
                string response = ReadSerialPort(serialPort);
                Console.WriteLine($"*IDN? response: {response}\n");

                // Set the frequency sweep parameters.
                WriteSerialPort(serialPort, "FREQ:STAR 2 GHz\n");
                WriteSerialPort(serialPort, "FREQ:STOP 4 GHz\n");
                WriteSerialPort(serialPort, "POW -10\n");
                WriteSerialPort(serialPort, "SWE1:STEP 100 MHz\n");                
                WriteSerialPort(serialPort, "SWE1:DWEL 1.3 s\n");
                WriteSerialPort(serialPort, "FREQ:MODE SWE1\n");

                // Turn on RF output.
                WriteSerialPort(serialPort, "OUTP ON\n");
                WriteSerialPort(serialPort, "TRIG:TYPE AUTO\n");

                // Trigger our way around the list
                while (true)
                {
                    
                }

            }
            catch (Exception e)
            {
                Console.WriteLine($"Exception {e.Message}.\n");
            }
            finally
            {
                serialPort.Close();
            }
        }
        static void OpenSerialPort(SerialPort serialPort)
        {
            serialPort.PortName = "COM4";
            serialPort.BaudRate = 115200;
            serialPort.Parity = Parity.None;
            serialPort.DataBits = 8;
            serialPort.StopBits = StopBits.One;
            serialPort.Handshake = Handshake.None;
            serialPort.ReadTimeout = 2000;

            try
            {
                serialPort.Open();
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error. Serial port open exception: {e.Message}.\n");
            }
        }

        static void WriteSerialPort(SerialPort serialPort, string commandString)
        {
            try
            {
                serialPort.Write(commandString);
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error. Serial port write exception: {e.Message}.\n");
            }
        }

        static string ReadSerialPort(SerialPort serialPort)
        {
            string response = "";
            try
            {
                response = serialPort.ReadLine();
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error. Serial port read exception: {e.Message}.\n");
            }

            return response;

        }
    }
}
