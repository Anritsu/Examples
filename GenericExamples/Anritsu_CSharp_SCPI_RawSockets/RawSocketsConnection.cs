using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;

namespace Anritsu_SCPI_RawSockets
{
    class Constants
    {
        public const int BufferLength = 1024;
        public const int OneByte = 1;
        public const char StartBlockChar = '#';
    }
    class RawSocketsConnection
    {
        private string ResourceName;
        private string IpAddress;
        private int PortNumber;
        private Socket Session;
        private string EndChar;

        public RawSocketsConnection(string resourceName)
        {
            if (String.IsNullOrEmpty(resourceName))
            {
                throw new Exception("Resource name could not be empty or null!");
            }
            ResourceName = resourceName;

            IpAddress = resourceName.Substring(resourceName.IndexOf("::") + 2, resourceName.LastIndexOf("::") - resourceName.IndexOf("::"));

            if (String.IsNullOrEmpty(IpAddress))
            {
                throw new Exception("Faulty resource name!");
            }

            string portNumberString = IpAddress.Substring(IpAddress.IndexOf("::") + 2, IpAddress.LastIndexOf("::") - IpAddress.IndexOf("::") - 2); //todo test?
            
            if (String.IsNullOrEmpty(portNumberString))
            {
                throw new Exception("Unable to extract the port number!");
            }

            PortNumber = Convert.ToInt32(portNumberString);

            IpAddress = IpAddress.Substring(0, IpAddress.IndexOf("::"));

            if (String.IsNullOrEmpty(IpAddress))
            {
                throw new Exception("Unable to identify the IP address!");
            }

            EndChar = "\n";
        }

        public void Connect(int timeOutMs)
        {
            try
            {
                IPAddress ipAddress = IPAddress.Parse(IpAddress);
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, PortNumber);

                Session = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
                Session.ReceiveTimeout = timeOutMs;
                Session.SendTimeout = timeOutMs;

                try
                {
                    Session.Connect(remoteEP);
                }
                catch (ArgumentNullException ane)
                {
                    Console.WriteLine("ArgumentNullException: {0} when trying to initialize socket connection {1}", ane.ToString(), ResourceName);
                }
                catch (SocketException se)
                {
                    Console.WriteLine("SocketException: {0} when trying to initialize socket connection {1}", se.ToString(), ResourceName);
                }
                catch (Exception e)
                {
                    Console.WriteLine("Unexpected Exception: {0} when trying to initialize socket connection {1}", e.ToString(), ResourceName);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected Exception: {0} when trying to create socket connection to {1}", e.ToString(), ResourceName);
            }

        }

        public void Disconnect()
        {
            try
            {
                Session.Shutdown(SocketShutdown.Both);
                Session.Close();
            }
            catch (ArgumentNullException ane)
            {
                Console.WriteLine("ArgumentNullException: {0} when trying to disconnect socket session from {1}", ane.ToString(), ResourceName);
            }
            catch (SocketException se)
            {
                Console.WriteLine("SocketException: {0} when trying to disconnect socket session from {1}", se.ToString(), ResourceName);
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected Exception: {0} when trying to disconnect socket session from {1}", e.ToString(), ResourceName);
            }
        }

        public string Query(string qCommand)
        {
            string response = "";
            Write(qCommand);
            try
            {
                /*Read, if applicable, the '#' character;
                 *For data-block readings*/
                byte[] firstCharBuffer = new byte[Constants.OneByte];
                Session.Receive(firstCharBuffer, 1, SocketFlags.None);
                response = Encoding.UTF8.GetString(firstCharBuffer);

                
                if (firstCharBuffer[0]== Constants.StartBlockChar)
                {
                    /*Read the next 1 character;
                    * it represent how many of the next digits make up the number of databytes to follow after it*/
                    byte[] sizeBuffer1 = new byte[Constants.OneByte];
                    Session.Receive(sizeBuffer1, Constants.OneByte, SocketFlags.None);
                    int blockDescriptionLen = sizeBuffer1[0] - 0x30;

                    //Read the data block size;
                    byte[] sizeBuffer2 = new byte[blockDescriptionLen];
                    Session.Receive(sizeBuffer2, blockDescriptionLen, SocketFlags.None);
                    int actualBlockDataLen = Int32.Parse(Encoding.UTF8.GetString(sizeBuffer2)) + 1;

                    //Read the data block
                    byte[] dataBuffer = new byte[actualBlockDataLen];
                    byte[] finalDataBuffer = new byte[actualBlockDataLen];
                    int bytesReceived = 0;
                    while (bytesReceived < actualBlockDataLen)
                    {
                        int oldBytesReceived = bytesReceived;
                        int receivedBytes = Session.Receive(dataBuffer, actualBlockDataLen - bytesReceived, SocketFlags.None);
                        Buffer.BlockCopy(dataBuffer, 0, finalDataBuffer, oldBytesReceived, receivedBytes);
                        bytesReceived += receivedBytes;
                    }
                    response = Encoding.UTF8.GetString(firstCharBuffer) + Encoding.UTF8.GetString(sizeBuffer1) + Encoding.UTF8.GetString(sizeBuffer2) + Encoding.UTF8.GetString(finalDataBuffer);
                    response = response.TrimEnd();
                    //response = Encoding.UTF8.GetString(finalDataBuffer).TrimEnd();
                }
                else
                {
                    byte[] dataBuffer = new byte[Constants.BufferLength];
                    int bytesReceived = Session.Receive(dataBuffer);
                    response += Encoding.ASCII.GetString(dataBuffer, 0, bytesReceived).TrimEnd("\r\n".ToCharArray());
                }
            }
            catch (SocketException se)
            {
                Console.WriteLine("SocketException : {0} when trying to receive response from command {1}", se.ToString(), qCommand);
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected exception :{0} when trying to receive response from command {1}", e.ToString(), qCommand);
            }

            return response.TrimEnd('\n');
        }

        public void SetTimeout(int timeOutMs)
        {
            Session.ReceiveTimeout = timeOutMs;
            Session.SendTimeout = timeOutMs;
        }

        public void Write(string wCommand)
        {
            try
            {
                byte[] message = Encoding.ASCII.GetBytes(wCommand + EndChar);
                int bytesSent = Session.Send(message);

                if (bytesSent != message.Length)
                {
                    throw new Exception("Failed to write the entire message!");
                }
            }
            catch (SocketException se)
            {
                Console.WriteLine("Socket Exception : {0} when trying to write {1}", se.ToString(), wCommand);
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected Exception : {0} when trying to write {1}", e.ToString(), wCommand);
            }
        }
    }
}
