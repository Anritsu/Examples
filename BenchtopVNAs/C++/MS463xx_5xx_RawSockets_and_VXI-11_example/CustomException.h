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

class SocketError : public CustomException
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

class SocketTimeoutSetReceiveError : public SocketTimeoutSetError
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

class SocketCommunicationError : public SocketError
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

class SocketCommunicationWriteError : public SocketCommunicationError
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

class SocketCommunicationReadError : public SocketCommunicationError
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

class SocketCommunicationTimeoutError : public SocketCommunicationReadError
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

class SocketConnectionError : public SocketError
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

class SocketConnectionConnectError : public SocketConnectionError
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

class SocketConnectionDisconnectError : public SocketConnectionError
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

class VisaTimeoutSetError : public std::exception //TODO: change ierarchy, this should not inherit directly from std::exception
{
public:
    VisaTimeoutSetError() : _exception() {}
    VisaTimeoutSetError(std::string exp) : _exception(exp) {}
    ~VisaTimeoutSetError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Visa Timeout set error";
    }
private:
    std::string _exception;
};

class VisaConnectionError : public std::exception //TODO: change ierarchy, this should not inherit directly from std::exception
{
public:
    VisaConnectionError() : _exception() {}
    VisaConnectionError(std::string exp) : _exception(exp) {}
    ~VisaConnectionError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Visa Connection Error";
    }
private:
    std::string _exception;
};

class VisaConnectionConnectError : public VisaConnectionError
{
public:
    VisaConnectionConnectError() : _exception() {}
    VisaConnectionConnectError(std::string exp) : _exception(exp) {}
    ~VisaConnectionConnectError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to connect to device";
    }
private:
    std::string _exception;
};

class VisaConnectionDisconnectError : public VisaConnectionError
{
public:
    VisaConnectionDisconnectError() : _exception() {}
    VisaConnectionDisconnectError(std::string exp) : _exception(exp) {}
    ~VisaConnectionDisconnectError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to connect to device";
    }
private:
    std::string _exception;
};

class VisaCommunicationError : public std::exception //TODO implement and inherit from VisaError
{
public:
    VisaCommunicationError() : _exception() {}
    VisaCommunicationError(std::string exp) : _exception(exp) {}
    ~VisaCommunicationError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Visa Communication Error";
    }
private:
    std::string _exception;
};

class VisaCommunicationWriteError : public VisaCommunicationError
{
public:
    VisaCommunicationWriteError() : _exception() {}
    VisaCommunicationWriteError(std::string exp) : _exception(exp) {}
    ~VisaCommunicationWriteError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to send command message to device";
    }
private:
    std::string _exception;
};

class VisaCommunicationReadError : public VisaCommunicationError
{
public:
    VisaCommunicationReadError() : _exception() {}
    VisaCommunicationReadError(std::string exp) : _exception(exp) {}
    ~VisaCommunicationReadError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to recieve command response from device";
    }
private:
    std::string _exception;
};

class VisaCommunicationTimeoutError : public VisaCommunicationReadError
{
public:
    VisaCommunicationTimeoutError() : _exception() {}
    VisaCommunicationTimeoutError(std::string exp) : _exception(exp) {}
    ~VisaCommunicationTimeoutError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Failed to recieve command response from device - Timeout Error";
    }
private:
    std::string _exception;
};