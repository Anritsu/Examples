classdef InstrumentVXI11Connection < handle
    %InstrumentVXI11Connection Summary:
    %   This class facilitates the use of VXI-11 connection functionality for users to command Anritsu VNA's using SCPI commands.
    % 	It uses the methods "connect" and "close" for starting and ending the connection for communication
	%	It uses the methods "SCPIwrite" and "SCPIquery" to send SCPI commands to the ShockLine software and read the reply if applicable
   
    properties
        ResourceName;
        Connection;
        Timeout;
        Ip;
    end
    
    methods
        
        function obj = InstrumentVXI11Connection(resource_name, timeout)
            obj.ResourceName = char(resource_name);
            collons = strfind(obj.ResourceName,"::");
            obj.Ip = obj.ResourceName(collons(1)+2:collons(2)-1);
            obj.Timeout = timeout;
        end
        
        function connect(obj)
            obj.Connection = instrfind('Type', 'visa-tcpip', 'RsrcName', ['TCPIP0::' obj.Ip '::inst0::INSTR'], 'Tag', '');
            
            if isempty(obj.Connection)
                obj.Connection = visa('NI', obj.ResourceName);
            else
                fclose(obj.Connection);
                obj.Connection = obj.Connection(1);
            end
            obj.Connection.InputBufferSize = 1000000;
            obj.Connection.OutputBufferSize = 1000000;
			set(obj.Connection, 'Timeout', obj.Timeout);
            fopen(obj.Connection);
        end
        
        function close(obj)
            fclose(obj.Connection);
        end
        
		function SCPIwrite(obj, write_message)
			fprintf(obj.Connection, write_message);
		end
		
		function response = SCPIquery(obj, query_message)
			response = query(obj.Connection, query_message);
			if response(1) == '#'
                response = response(12:end);
			end
        end
    end
    
end

