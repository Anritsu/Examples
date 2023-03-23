import java.net.*;
import java.nio.charset.StandardCharsets;
import java.io.*;

public class RawSocketsConnection {
    private String ResourceName;
    private String IpAddress;
    private int PortNumber;
    private Socket Session;
    private String EndChar;
    private BufferedReader SessionInStream; // Used for reading of single line responses from hardware.
    private DataInputStream SessionInStream2; // Used for reading arbitrary block data responses from hardware, by reading the exact number of indicated bytes.
    private PrintStream SessionOutStream;
    private boolean Connected = false;

    public RawSocketsConnection(String resourceName)
    {
        ResourceName = resourceName;
        if(ResourceName.isBlank() || ResourceName.isEmpty() || ResourceName == null)
        {
            try {
                throw new Exception("Resource name could not be empty or null!");
            } catch (Exception e) {
                System.out.println(e.getMessage());
                System.exit(-1);
            }
        }

        IpAddress = ResourceName.substring(ResourceName.indexOf("::") + 2, ResourceName.lastIndexOf("::") + 2);
        if(IpAddress.isBlank() || IpAddress.isEmpty() || IpAddress == null)
        {
            try {
                throw new Exception("Faulty resource name!");
            } catch (Exception e) {
                System.out.println(e.getMessage());
                System.exit(-1);
            }
        }

        String portNumberString = IpAddress.substring(IpAddress.indexOf("::") + 2, IpAddress.lastIndexOf("::"));
        if(portNumberString.isBlank() || portNumberString.isEmpty() || portNumberString == null)
        {
            try {
                throw new Exception("Unable to extract the port number!");
            } catch (Exception e) {
                System.out.println(e.getMessage());
                System.exit(-1);
            }
        }

        PortNumber = Integer.parseInt(portNumberString);

        IpAddress = IpAddress.substring(0, IpAddress.indexOf("::"));
        if(IpAddress.isBlank() || IpAddress.isEmpty() || IpAddress == null)
        {
            try {
                throw new Exception("Unable to identify the IP address!");
            } catch (Exception e) {
                System.out.println(e.getMessage());
                System.exit(-1);
            }
        }
        
        EndChar = "\n";
        Connected = false;
    }

    public void Connect(int timeOutMs)
    {
        try 
        {
            if(!Connected)
            {
                Session = new Socket(IpAddress, PortNumber);
                Session.setSoTimeout(timeOutMs);
    
                SessionOutStream = new PrintStream(Session.getOutputStream());
                SessionInStream = new BufferedReader(new InputStreamReader(Session.getInputStream()));
                SessionInStream2 = new DataInputStream(Session.getInputStream());
                Connected = true;   
            }
        }
        catch(UnknownHostException uhe) {
            System.out.println(String.format("Faulty Host IP address ('%s').", IpAddress));
            System.out.println(uhe.getMessage());
            System.exit(-1);
        }
        catch (IOException ioe){
            System.out.println(String.format("I/O connect operation to socket %s::%d failed.",IpAddress, PortNumber));
            System.out.println(ioe.getMessage());
            System.exit(-1);
        }
    }

    public void Disconnect()
    {
        try
        {
            if(Connected)
            {
                SessionOutStream.println();
                SessionOutStream.flush();
                SessionInStream.close();
                SessionOutStream.close();
                Session.close();
                Connected = false;
            }
        }
        catch (IOException ioe){
            System.out.println(String.format("I/O disconnect operation to socket %s::%d failed.",IpAddress, PortNumber));
            System.out.println(ioe.getMessage());
            System.exit(-1);
        }
    }

    public void Write(String wcommand)
    {
        try{
            if (Connected==true)
            {
                String SCPICommand = wcommand + EndChar;
                SessionOutStream.print(SCPICommand);
                if (SessionOutStream.checkError())
                {
                    throw new Exception("Write error");
                }
                SessionOutStream.flush();
            }
        }
        catch (Exception e)
        {
            System.out.println(String.format("I/O write operation of '%s' to socket %s::%d failed.",wcommand ,IpAddress, PortNumber));
            System.exit(-1);
        }
    }

    public String Query(String qcommand)
    {
        Write(qcommand);
        try {
            byte[] oneByte = new byte[1];
            oneByte[0] = SessionInStream2.readByte();
            String firstCharString = new String(oneByte,StandardCharsets.UTF_8);
            char[] firstChar = new char[1];
            firstCharString.getChars(0,1,firstChar,0);
            
            if (firstChar[0] == '#')
            {
                byte[] sizeBuffer1 = new byte[1];
                sizeBuffer1[0] = SessionInStream2.readByte();
                int blockDescriptionLen = sizeBuffer1[0] - 0x30;

                //Read the data block size;
                byte[] sizeBuffer2 = new byte[blockDescriptionLen];
                for (int i = 0; i < blockDescriptionLen ; i++)
                {
                    sizeBuffer2[i] = SessionInStream2.readByte();
                }

                String actualBlockDataLenString = new String(sizeBuffer2, StandardCharsets.UTF_8);
                int BlockDataLen = Integer.valueOf(actualBlockDataLenString);

                
                //Read the data block
                byte[] dataBuffer = new byte[BlockDataLen];
                for (int i = 0; i < BlockDataLen ; i++)
                {
                    dataBuffer[i] = SessionInStream2.readByte();
                }

                //Read last empty byte
                byte[] lastByte = new byte[1];
                lastByte[0] = SessionInStream2.readByte();
                String lastByteString = new String(lastByte, StandardCharsets.UTF_8);

                String dataBlock = new String(dataBuffer,StandardCharsets.UTF_8);

                String response = firstCharString + blockDescriptionLen + actualBlockDataLenString + dataBlock + lastByteString;

                return response.trim();
            }
            else
            {
                String response = SessionInStream.readLine();
                response = firstCharString + response;
                return response.trim();
            }
        } catch (IOException ioe) 
        {
            System.out.println(String.format("I/O read operation after '%s' to socket %s::%d failed.",qcommand ,IpAddress, PortNumber));
            System.out.println(ioe.getMessage());
            System.exit(-1);
        }
        return null;
    }
}
