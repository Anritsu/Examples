using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace LIST
{
    class Program
    {
        static SerialPort serialPort;

        static void Main(string[] args)
        {
            Console.WriteLine("Start LIST - Serial - SCPI\n");

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

                // Create the frequency and power lists.
                WriteSerialPort(serialPort, "LIST1:IND 0\n");
                WriteSerialPort(serialPort, "LIST1:FREQ 2.1GHz, 2.2GHz, 2.3GHz, 2.5GHz, 2.8GHz, 3GHz, 3.1GHz, 3.2GHz, 3.3GHz, 3.5GHz, 3.8GHz\n");
                WriteSerialPort(serialPort, "LIST1:POW  -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5\n");
                WriteSerialPort(serialPort, "LIST1:STAR 0; :LIST1:STOP 10\n");
                WriteSerialPort(serialPort, "\n");
                WriteSerialPort(serialPort, "TRIG:TYPE MAN\n");
                WriteSerialPort(serialPort, "LIST1:IND 10\n");
                WriteSerialPort(serialPort, "FREQ:MODE LIST1\n");

                // Turn on RF output.
                WriteSerialPort(serialPort, "OUTP ON\n");

                // Trigger our way around the list
                while (true) 
                {
                    // Trigger to nex freq
                    WriteSerialPort(serialPort, "*TRG\n");

                    // Wait for *OPC?
                    WriteSerialPort(serialPort, "*OPC?\n");
                    response = ReadSerialPort(serialPort);
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
