
%% Object instantiation
AnritsuSCPIConnection = tcpip('127.0.0.1', 5001, 'Timeout', 5);
AnritsuSCPIConnection.InputBufferSize = 4096;

%% 0. Instrument connection
fopen(AnritsuSCPIConnection);

%% 1. *IDN Query
fprintf(AnritsuSCPIConnection, "*IDN?\n");
idn = fscanf(AnritsuSCPIConnection);
fprintf (idn);

%% 2. System Preset
fprintf(AnritsuSCPIConnection, '*RST\n');

%Block Data has to be read with - readBlockValue function
%fdata = readBlockValue(AnritsuSCPIConnection, ':CALC1:PAR1:DATA:FDAT?\n');
%fprintf (fdata);

%% 3. Connection closing
fclose(AnritsuSCPIConnection);
delete(AnritsuSCPIConnection);
clear AnritsuSCPIConnection;


%% Read block data function
function retVal = readDataBlockSize(x)
    retVal = str2double(extractBetween(x, 3, 2 + str2double(x(2))));
end

function retVal = readBlockValue(vna, scpiCmd)
    fprintf(vna, scpiCmd);
    retVal = fscanf(vna, '%c', 11);   
    dataLen = readDataBlockSize(retVal);
    actualDataLen = 0;
    retVal = '';
    while actualDataLen < dataLen
        data = fscanf(vna);
        actualDataLen = actualDataLen + length(data);
        retVal = sprintf('%s%s', retVal, data);
    end    
end 
