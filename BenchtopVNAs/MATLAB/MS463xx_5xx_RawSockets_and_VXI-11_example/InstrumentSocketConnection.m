classdef InstrumentSocketConnection < handle
    %VXIINSTRUMENTCLASS Summary:
    %   This class facilitates the use of RawSocket connection functionality for users to command Anritsu VNA's using SCPI commands.
    % 	It uses the methods "connect" and "close" for starting and ending the connection for communication
	%	It uses the methods "SCPIwrite" and "SCPIquery" to send SCPI commands to the ShockLine software and read the reply if applicable
	
    properties
		ResourceName;
        SocketConnection;
        Timeout;
        Ip;
        Port;
        NewLine;
    end
    
    methods
        function obj = InstrumentSocketConnection(resource_name, timeout)
            obj.NewLine = '\n';
			obj.ResourceName = char(resource_name);
            collons = strfind(obj.ResourceName,"::");
            obj.Ip = obj.ResourceName(collons(1)+2:collons(2)-1);
            obj.Port = str2num(obj.ResourceName(collons(2)+2:collons(3)-1));
            obj.Timeout = timeout;
        end
        
        function SCPIwrite(obj, write_message)
            fprintf(obj.SocketConnection, [write_message obj.NewLine]);
        end
        
        function response = SCPIquery(obj, query_message)
            fprintf(obj.SocketConnection, [query_message obj.NewLine]);
            response = fscanf(obj.SocketConnection);
            if response(1)=='#'
                datablocksize = str2double(extractBetween(response, 3, 2 + str2double(response(2))));
                response = char(extractBetween(response, 3 + str2double(response(2)), length(response)));
                actualDataLen = length(response);
                while actualDataLen < datablocksize
                    data = fscanf(obj.SocketConnection);
                    actualDataLen = actualDataLen + length(data);
                    response = sprintf('%s%s', response, data);
                end
            end
        end
   
        function connect(obj)
            obj.SocketConnection = tcpip(obj.Ip, obj.Port, 'Timeout', obj.Timeout);
            obj.SocketConnection.InputBufferSize = 4096;
			fopen(obj.SocketConnection);
        end
        
        function close(obj)
            fclose(obj.SocketConnection);
            delete(obj.SocketConnection);
            clear obj.SocketConnection;
        end 
        
    end
    
end
