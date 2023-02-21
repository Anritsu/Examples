#pragma once

#include <exception>
#include <string>
class VirtualCustomException :
    public std::exception
{
public:
    VirtualCustomException() : _exception() {}
    VirtualCustomException(std::string exp) : _exception(exp) {}
    ~VirtualCustomException() throw () {}
    const std::string toString() const throw() { return _exception; }
    virtual const char* what() = 0;
private:
    std::string _exception;
};

class CustomException : public VirtualCustomException
{
public:
    using VirtualCustomException::VirtualCustomException;
    const char* what() {
        return "Custom error";
    }
};

class SocketError : public CustomException
{
public:
    using CustomException::CustomException;
    const char* what() {
        return "Socket Error";
    }
private:
    std::string _exception;
};

class SocketTimeoutSetError : public SocketError
{
public:
    using SocketError::SocketError;
    const char* what() {
        return "Socket Send/Recieve Timeout Set Error";
    }
};

class SocketTimeoutSetSendError : public SocketTimeoutSetError
{
public:
    using SocketTimeoutSetError::SocketTimeoutSetError;
    const char* what() {
        return "Socket send Timeout Set Error";
    }
};

class SocketTimeoutSetReceiveError : public SocketTimeoutSetError
{
public:
    using SocketTimeoutSetError::SocketTimeoutSetError;
    const char* what() {
        return "Socket recieve Timeout Set Error";
    }
};

class SocketCommunicationError : public SocketError
{
public:
    using SocketError::SocketError;
    const char* what() {
        return "Socket Communication Error";
    }
};

class SocketCommunicationWriteError : public SocketCommunicationError
{
public:
    using SocketCommunicationError::SocketCommunicationError;
    const char* what() {
        return "Socket Communication Write Error";
    }
};

class SocketCommunicationReadError : public SocketCommunicationError
{
public:
    using SocketCommunicationError::SocketCommunicationError;
    const char* what() {
        return "Socket Communication Read Error";
    }
};

class SocketCommunicationTimeoutError : public SocketCommunicationReadError
{
public:
    using SocketCommunicationReadError::SocketCommunicationReadError;
    const char* what() {
        return "Socket Communication Read Timeout Error";
    }
};

class SocketConnectionError : public SocketError
{
public:
    using SocketError::SocketError;
    const char* what() {
        return "Socket Connection Error";
    }
};

class SocketConnectionConnectError : public SocketConnectionError
{
public:
    using SocketConnectionError::SocketConnectionError;
    const char* what() {
        return "Socket Connection Connect Error";
    }
};

class SocketConnectionDisconnectError : public SocketConnectionError
{
public:
    using SocketConnectionError::SocketConnectionError;
    const char* what() {
        return "Socket Connection Disconnect Error";
    }
};

class VisaError : public CustomException
{
public:
    using CustomException::CustomException;
    const char* what() {
        return "Visa Error";
    }
};

class VisaTimeoutSetError : public VisaError
{
public:
    using VisaError::VisaError;
    const char* what() {
        return "Visa Timeout Set Error";
    }
};


class VisaConnectionError : public VisaError
{
public:
    using VisaError::VisaError;
    const char* what() {
        return "Visa Connection Error";
    }
};

class VisaConnectionConnectError : public VisaConnectionError
{
public:
    using VisaConnectionError::VisaConnectionError;
    const char* what() {
        return "Visa Connection Connect Error";
    }
};

class VisaConnectionDisconnectError : public VisaConnectionError
{
public:
    using VisaConnectionError::VisaConnectionError;
    const char* what() {
        return "Visa Connection Disconnect Error";
    }
};

class VisaCommunicationError : public VisaError
{
public:
    using VisaError::VisaError;
    const char* what() {
        return "Visa Communication Error";
    }
};

class VisaCommunicationWriteError : public VisaCommunicationError
{
public:
    using VisaCommunicationError::VisaCommunicationError;
    const char* what() {
        return "Visa Communication Write Error";
    }
};

class VisaCommunicationReadError : public VisaCommunicationError
{
public:
    using VisaCommunicationError::VisaCommunicationError;
    const char* what() {
        return "Visa Communication Read Error";
    }
};

class VisaCommunicationTimeoutError : public VisaCommunicationReadError
{
public:
    using VisaCommunicationReadError::VisaCommunicationReadError;
    const char* what() {
        return "Visa Communication Read Timeout Error";
    }
};