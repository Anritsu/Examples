using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;

namespace CW
{
    class Program
    {
        static SerialPort serialPort;
        static byte[] responseBytes;
        static byte[] COMMAND_LANG_QSYN = { 0x53, 0x59, 0x53, 0x54, 0x3A, 0x4C, 0x41, 0x4E, 0x47, 0x20, 0x51, 0x53, 0x59, 0x4E, 0x0A }; // SYST:LANG QSYN\n
        static byte[] COMMAND_RECALL_DEFAULT = { 0x32, 0x37, 0x30, 0x30 }; // 0x2700
        static byte[] QUERY_ID = { 0x30, 0x31 }; // 0x01
        const int QUERY_ID_NUM_RESPONSE = 22;
        static byte[] COMMAND_RF_ON = { 0x30, 0x46, 0x30, 0x31 }; // 0x0F01
        static byte[] COMMAND_FREQ = { 0x30, 0x43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // 0x0C<space for 12 bytes of freq hex ASCII>

        static void Main(string[] args)
        {
            Console.WriteLine("Start CW - Serial - QSYN LANG\n");

            // Assumptions:
            // 1. 

            try
            {
                // Open the serial port.
                serialPort = new SerialPort();
                OpenSerialPort(serialPort);

                // Set QSYN language and then delay to allow time for the switch.
                WriteSerialPort(serialPort, COMMAND_LANG_QSYN);
                Thread.Sleep(100);

                // Query ID & sleep to allow time for the response.
                WriteSerialPort(serialPort, QUERY_ID);
                Thread.Sleep(50);
                

                // Read the response.
                responseBytes = new byte[80];
                int numBytesRead = ReadSerialPort(serialPort, responseBytes, QUERY_ID_NUM_RESPONSE);
#if DEBUG
                if(numBytesRead != QUERY_ID_NUM_RESPONSE)
                {
                    Console.WriteLine($"Error: read ID num read {numBytesRead} != num expected {QUERY_ID_NUM_RESPONSE}");
                }
                for (int i = 0; i < numBytesRead; i++)
                {
                    Console.WriteLine("{0,2:X}", responseBytes[i]);
                }
#endif
                // Preset
                WriteSerialPort(serialPort, COMMAND_RECALL_DEFAULT);
                Thread.Sleep(100);


                // Loop through some CW freq changes.
                ulong startFreqMilliHz = 1000000000000;
                ulong stopFreqMilliHz = 5000000000000;
                ulong stepMilliHz = 100000000000;
                ulong currentFreq = startFreqMilliHz;

                // Turn on RF output.
                WriteSerialPort(serialPort, COMMAND_RF_ON);

                while (true) //((stopFreqGHz - currentFreq) > 0.001)
                {
                    // Convert ulong frequency to hex string.
                    string freqHexString;
                    freqHexString = string.Format("{0:X12}", currentFreq);
                    for(int i=0; i<12; i++)
                    {
                        int commandIndex = i + 2;
                        COMMAND_FREQ[commandIndex] = (byte)freqHexString[i];
                    }


                    // Set CW freq
                    WriteSerialPort(serialPort, COMMAND_FREQ);
                    Thread.Sleep(100);


                    // Increment freq
                    currentFreq += stepMilliHz;
                    if (currentFreq > stopFreqMilliHz)
                    {
                        currentFreq = startFreqMilliHz;
                    }
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

        static void WriteSerialPort(SerialPort serialPort, byte[] commandBytes)
        {
            try
            {
                serialPort.Write(commandBytes, 0, commandBytes.Length);
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error. Serial port write exception: {e.Message}.\n");
            }
        }

        static int ReadSerialPort(SerialPort serialPort, byte[] responseBytes, int numResponseBytesExpected)
        {
            int numBytesRead = 0;
            try
            {
                numBytesRead = serialPort.Read(responseBytes, 0, numResponseBytesExpected);
                if(numBytesRead != numResponseBytesExpected)
                {
                    throw new Exception($"Num bytes read {numBytesRead} doesn't equal num bytes expected {numResponseBytesExpected}");
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"Error. Serial port read exception: {e.Message}.\n");
            }

            return numBytesRead;

        }
    }
}
