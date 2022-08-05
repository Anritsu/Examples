classdef InstrumentConnection < handle
    %INSTRUMENTCONNECTION Summary:
    %   This class facilitates the use of RawSocket or VXI-11 connection functionality for users to command Anritsu VNA's using SCPI commands.
	%	The connection type is automatically determined based on the resource name
    
    
    properties
        ResourceName;
        Connection;
        Timeout;
        ResourceType;
    end
    
    methods
        
        function obj = InstrumentConnection(resource_name, timeout)
            obj.ResourceName = char(resource_name);
			obj.Timeout = timeout;
            collons = strfind(obj.ResourceName,"::");
            obj.ResourceType = obj.ResourceName(collons(3)+2:end);
		end
		
		function connect(obj)
			if (strcmp(obj.ResourceType,'SOCKET'))
				obj.Connection = InstrumentSocketConnection(obj.ResourceName, obj.Timeout);	
			elseif (strcmp(obj.ResourceType,'INSTR'))
				obj.Connection = InstrumentVXI11Connection(obj.ResourceName, obj.Timeout);
			end	
			obj.Connection.connect();
        end
		
		function close(obj)
            obj.Connection.close();
        end
		
		function SCPIwrite(obj, write_message)
			obj.Connection.SCPIwrite(write_message);
		end
		function response = SCPIquery(obj, query_message)
			response = obj.Connection.SCPIquery(query_message);
		end
    end
    
end

