#include "SCPIConnection.h"

SCPIConnection::SCPIConnection(std::string resourceName) : ResourceName(resourceName), Connected(false)
{
}

SCPIConnection::~SCPIConnection()
{
}
