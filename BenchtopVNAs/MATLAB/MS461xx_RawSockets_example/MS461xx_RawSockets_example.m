resource_name = 'TCPIP0::127.0.0.1::5001::SOCKET';
timeout = 10; % in seconds
opc_passed = ['1' newline];

ShockLine_Socket = InstrumentSocketConnection(resource_name, timeout);

%% 0. Instrument connection
ShockLine_Socket.connect;

%% 1. Read Instrument Type
idn = ShockLine_Socket.SCPIquery('*IDN?');
fprintf(idn);
fprintf(['Instrument belongs to the ' idn(13) '00''s family' newline]);

%% 2. System Preset
ShockLine_Socket.SCPIwrite(':SYST:PRES');

%% 3. Define 4 traces - S11, S21, S12 & S22
ShockLine_Socket.SCPIwrite(':CALC1:PAR:COUN 4');

ShockLine_Socket.SCPIwrite(':CALC1:PAR1:DEF S11');
ShockLine_Socket.SCPIwrite(':CALC1:PAR2:DEF S21');
ShockLine_Socket.SCPIwrite(':CALC1:PAR3:DEF S12');
ShockLine_Socket.SCPIwrite(':CALC1:PAR4:DEF S22');

%% 4. Define LOG MAG response
ShockLine_Socket.SCPIwrite(':CALC1:PAR1:FORM MLOG');
ShockLine_Socket.SCPIwrite(':CALC1:PAR2:FORM MLOG');
ShockLine_Socket.SCPIwrite(':CALC1:PAR3:FORM MLOG');
ShockLine_Socket.SCPIwrite(':CALC1:PAR4:FORM MLOG');

%% 5. Set frequency range 1GHz - 4GHz
ShockLine_Socket.SCPIwrite(':SENS1:FREQ:STAR 1e9');
ShockLine_Socket.SCPIwrite(':SENS1:FREQ:STOP 4e9');

%% 6. Set 501 poins
ShockLine_Socket.SCPIwrite(':SENS1:SWE:POIN 501');

%% 7. Set 100KHz IFBW
ShockLine_Socket.SCPIwrite(':SENS1:BAND 1E5');

%% 8. Set instrument on Hold
ShockLine_Socket.SCPIwrite(':SENS1:HOLD:FUNC HOLD');

%% 9. Perform a SOLT calibration
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:METH SOLT');
calibration_method = ShockLine_Socket.SCPIquery(':SENS1:CORR:COLL:METH?');
fprintf(['The Calibration method used is: ' calibration_method]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT12:FULL2');
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT1:CONN CFKT');
connector_type_port_1 = ShockLine_Socket.SCPIquery(':SENS1:CORR:COLL:PORT1:CONN?');
fprintf(['Port1 Connector set to: ' connector_type_port_1]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT2:CONN CFKT');
connector_type_port_2 = ShockLine_Socket.SCPIquery(':SENS1:CORR:COLL:PORT2:CONN?');
fprintf(['Port2 Connector set to: ' connector_type_port_2]);

input(['Please connect the LOAD to port 1 and press ENTER to continue' newline]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT1:LOAD');

input(['Please connect the OPEN to port 1 and press ENTER to continue' newline]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT1:OPEN');

input(['Please connect the SHORT to port 1 and press ENTER to continue' newline]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT1:SHORT');

input(['Please connect the LOAD to port 2 and press ENTER to continue' newline]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT2:LOAD');

input(['Please connect the OPEN to port 2 and press ENTER to continue' newline]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT2:OPEN');

input(['Please connect the SHORT to port 2 and press ENTER to continue' newline]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT2:SHORT');

input(['Please connect a THRU between port 1 and port 2 and press ENTER to continue' newline]);
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:PORT12:THRU');
ShockLine_Socket.SCPIwrite(':SENS1:CORR:COLL:SAVE');

opc_done = ShockLine_Socket.SCPIquery('*OPC?');
if not(opc_done(1) == opc_passed(1))
    disp(['Error, Opc returned unexpected value while waiting for calibration to finish (expected "' strrep(opc_passed,newline,'\n') '", received "' strrep(opc_done,newline,'\n') '"); ending code execution.']);
    ShockLine_Socket.close;
    return
end
clear opc_done;

input(['Please connect DUT and press ENTER to continue' newline]);

%% 10. Perform a sweep and wait until done
ShockLine_Socket.SCPIwrite(':TRIG:SING');
opc_done = ShockLine_Socket.SCPIquery('*OPC?');
if not(opc_done(1) == opc_passed(1))
    fprintf(['Error, Opc returned unexpected value while waiting for a single sweep to finish (expected "' strrep(opc_passed,newline,'\n') '", received "' strrep(opc_done,newline,'\n') '"); ending code execution.']);
    ShockLine_Socket.close;
    return
end
clear opc_done;

%% 11. Read Frequecy list
freqData = ShockLine_Socket.SCPIquery(':SENS1:FREQ:DATA?');
fprintf(freqData);

%% 12. Read S Parameters
disp("Param 1 - S11 - SData");
S11SData = ShockLine_Socket.SCPIquery(':CALC1:PAR1:DATA:SDAT?');
fprintf(S11SData);

disp("Param 2 - S21 - SData");
S21SData = ShockLine_Socket.SCPIquery(':CALC1:PAR2:DATA:SDAT?');
fprintf(S21SData);

disp("Param 3 - S12 - SData");
S12SData = ShockLine_Socket.SCPIquery(':CALC1:PAR3:DATA:SDAT?');
fprintf(S12SData);

disp("Param 4 - S22 - SData");
S22SData = ShockLine_Socket.SCPIquery(':CALC1:PAR4:DATA:SDAT?');
fprintf(S22SData);

%% 13. Read Formatted Data
disp("Param 1 - S11 - FData");
S11FData = ShockLine_Socket.SCPIquery(':CALC1:PAR1:DATA:FDAT?');
fprintf(S11FData);

disp("Param 2 - S21 - FData");
S21FData = ShockLine_Socket.SCPIquery(':CALC1:PAR2:DATA:FDAT?');
fprintf(S21FData);

disp("Param 3 - S12 - FData");
S12FData = ShockLine_Socket.SCPIquery(':CALC1:PAR3:DATA:FDAT?');
fprintf(S12FData);

disp("Param 4 - S22 - FData");
S22FData = ShockLine_Socket.SCPIquery(':CALC1:PAR4:DATA:FDAT?');
fprintf(S22FData);

%% 14. Connection closing
ShockLine_Socket.close;

%% Clearing temporary variables from Workspace
clear opc_passed;
clear idn;
clear calibration_method;
clear ShockLine_Socket ipAddress resource_name timeout;
clear connector_type_port_1 connector_type_port_2;