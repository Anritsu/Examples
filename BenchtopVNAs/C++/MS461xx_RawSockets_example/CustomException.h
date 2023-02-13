#pragma once

#include <exception>
#include <string>
class CustomException :
    public std::exception
{
public:
    CustomException() : _exception() {}
    CustomException(std::string exp) : _exception(exp) {}
    ~CustomException() throw () {}
    const std::string toString() const throw() { return _exception; }
private:
    std::string _exception;
};

class SocketError : CustomException
{
public:
    SocketError() : _exception() {}
    SocketError(std::string exp) : _exception(exp) {}
    ~SocketError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Socket Error";
    }
private:
    std::string _exception;
};

class SocketTimeoutSetError : public SocketError
{
public:
    SocketTimeoutSetError() : _exception() {}
    SocketTimeoutSetError(std::string exp) : _exception(exp) {}
    ~SocketTimeoutSetError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Socket Timeout set and/or send error";
    }
private:
    std::string _exception;
};

class SocketTimeoutSetSendError : public SocketTimeoutSetError
{
public:
    SocketTimeoutSetSendError() : _exception() {}
    SocketTimeoutSetSendError(std::string exp) : _exception(exp) {}
    ~SocketTimeoutSetSendError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to set send timeout";
    }
private:
    std::string _exception;
};

class SocketTimeoutSetReceiveError : SocketTimeoutSetError
{
public:
    SocketTimeoutSetReceiveError() : _exception() {}
    SocketTimeoutSetReceiveError(std::string exp) : _exception(exp) {}
    ~SocketTimeoutSetReceiveError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to set recieve timeout";
    }
private:
    std::string _exception;
};

class SocketCommunicationError : SocketError
{
public:
    SocketCommunicationError() : _exception() {}
    SocketCommunicationError(std::string exp) : _exception(exp) {}
    ~SocketCommunicationError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Socket Communication Error";
    }
private:
    std::string _exception;
};

class SocketCommunicationWriteError : SocketCommunicationError
{
public:
    SocketCommunicationWriteError() : _exception() {}
    SocketCommunicationWriteError(std::string exp) : _exception(exp) {}
    ~SocketCommunicationWriteError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to send command message to device";
    }
private:
    std::string _exception;
};

class SocketCommunicationReadError : SocketCommunicationError
{
public:
    SocketCommunicationReadError() : _exception() {}
    SocketCommunicationReadError(std::string exp) : _exception(exp) {}
    ~SocketCommunicationReadError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to recieve command response from device";
    }
private:
    std::string _exception;
};

class SocketCommunicationTimeoutError : SocketCommunicationReadError
{
public:
    SocketCommunicationTimeoutError() : _exception() {}
    SocketCommunicationTimeoutError(std::string exp) : _exception(exp) {}
    ~SocketCommunicationTimeoutError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to recieve command response from device - Timeout Error";
    }
private:
    std::string _exception;
};

class SocketConnectionError : SocketError
{
public:
    SocketConnectionError() : _exception() {}
    SocketConnectionError(std::string exp) : _exception(exp) {}
    ~SocketConnectionError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Socket Connection Error";
    }
private:
    std::string _exception;
};

class SocketConnectionConnectError : SocketConnectionError
{
public:
    SocketConnectionConnectError() : _exception() {}
    SocketConnectionConnectError(std::string exp) : _exception(exp) {}
    ~SocketConnectionConnectError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to connect to device";
    }
private:
    std::string _exception;
};

class SocketConnectionDisconnectError : SocketConnectionError
{
public:
    SocketConnectionDisconnectError() : _exception() {}
    SocketConnectionDisconnectError(std::string exp) : _exception(exp) {}
    ~SocketConnectionDisconnectError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to disconnect from device";
    }
private:
    std::string _exception;
};