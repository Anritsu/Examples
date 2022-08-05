using System;
using NationalInstruments.Visa;
using Ivi.Visa;

namespace MS463xx_5xx_RawSockets_or_VXI_11_ex
{
    class NIVXI11Connection : SCPIConnection
    {
        private MessageBasedSession Session;
        private string ResourceName;


        public NIVXI11Connection(string resourceName)
        {
            if (String.IsNullOrEmpty(resourceName))
            {
                throw new Exception("Resource name could not be empty or null!");
            }

                Session = null;

            ResourceName = resourceName;

            if (ResourceName.Contains("::SOCKET"))
            {
                throw new Exception("Resource name must not be an address reffering to a SOCKET connection!");
            }
            else if (!ResourceName.Contains("::INSTR"))
            {
                throw new Exception("Resource name must be an address reffering to a INSTR connection!");
            }
        }
        public void Connect(int timeOutMs)
        {
            try
            {
                ResourceManager rm = new ResourceManager();
                Session = (MessageBasedSession)rm.Open(ResourceName);
                Session.TimeoutMilliseconds = timeOutMs;
                
                switch (Session.HardwareInterfaceType)
                {
                    case HardwareInterfaceType.Tcp:
                        Session.TerminationCharacterEnabled = true;
                        break;
                    default:
                        break;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected exception : {0} when trying to connect to {1}", e.ToString(), ResourceName);
            }
        }

        public void Disconnect()
        {
            try
            {
                if(Session != null)
                {
                    Session.Dispose();
                    Session = null;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected exception : {0} when trying to disconnect from {1}", e.ToString(), ResourceName);
            }
        }

        public string Query(string qCommand)
        {
            string response = "";
            try
            {
                Session.FormattedIO.WriteLine(qCommand);
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected exception : {0} when trying to send query command {1}", e.ToString(), qCommand);
            }
            try
            {
                // Read first character, determine if it's "#", preceding block data
                response += Session.FormattedIO.ReadChar();

                // If not, read the rest of the line
                if (response != Constants.StartBlockChar.ToString())
                    response += Session.FormattedIO.ReadLine();
                // Else...
                else
                {
                    string sizeBuffer1 = "";
                    string sizeBuffer2 = "";
                    // Read data block size descriptor
                    sizeBuffer1 = Convert.ToString(Session.FormattedIO.ReadChar());
                    response += sizeBuffer1;

                    // Read data block size
                    for(int i = 0; i< Convert.ToInt32(sizeBuffer1); i++)
                    {
                        sizeBuffer2 += Session.FormattedIO.ReadChar();
                    }
                    response += sizeBuffer2;

                    // Read data block according to the size determined + 1 final character (newline)
                    for (int i = 0; i <= Convert.ToInt32(sizeBuffer2); i++)
                    {
                        response += Session.FormattedIO.ReadChar();
                    }

                    // Optional Buffer Discard for no interference in future query reads
                    Session.FormattedIO.DiscardBuffers(); // to investigate further
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected exception : {0} when trying to read the response to query command {1}", e.ToString(), qCommand);
            }

            return response.TrimEnd('\n');
        }

        public void SetTimeout(int timeOutMs)
        {
            Session.TimeoutMilliseconds = timeOutMs;
        }

        public void Write(string wCommand)
        {
            try
            {
                Session.FormattedIO.WriteLine(wCommand);
            }
            catch (Exception e)
            {
                Console.WriteLine("Unexpected exception : {0} when trying to write {1}", e.ToString(), wCommand);
            }
        }
    }
}
