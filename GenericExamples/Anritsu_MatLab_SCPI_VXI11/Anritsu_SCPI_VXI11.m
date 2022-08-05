%% Object instantiation
resourceName = "TCPIP0::127.0.0.1::inst0::INSTR";
AnritsuSCPIConnection = visa('NI', resourceName);
AnritsuSCPIConnection.InputBufferSize = 1000000;
AnritsuSCPIConnection.OutputBufferSize = 1000000;


%% 0. Instrument connection
fopen(AnritsuSCPIConnection);

%% 1. *IDN Query
idn = query(AnritsuSCPIConnection, '*IDN?');
fprintf(idn);

%% 2. System Preset
fprintf(AnritsuSCPIConnection, '*RST');

%% 3. Connection closing
fclose(AnritsuSCPIConnection);
