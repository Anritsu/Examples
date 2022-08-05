#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "SCPIConnection.h"

class RawSocketConnection :
    public SCPIConnection
{
public:
    RawSocketConnection(std::string resourceName);
    ~RawSocketConnection();

    void Connect();
    void Disconnect();
    int Write(std::string w_command);
    std::string Query(std::string q_cmd);
    void SetTimeout(int timeout);

private:
    WSADATA WSA;
    SOCKET Socket;
    struct sockaddr_in Server;

    int PortNo;
    char IpAddress[256];
};

