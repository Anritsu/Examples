#pragma once
#include "SCPIConnection.h"
#include "visa.h"
class VXIConnection :
    public SCPIConnection
{
public:
    VXIConnection(std::string resourceName);
    ~VXIConnection();

    void Connect();
    void Disconnect();
    int Write(std::string w_command);
    std::string Query(std::string q_cmd);
    void SetTimeout(int timeout);

private:
    ViSession ResourceManager;
    ViSession Session;
};

