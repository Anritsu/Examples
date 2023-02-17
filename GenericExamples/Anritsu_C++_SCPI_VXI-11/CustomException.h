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

class VisaError : public CustomException
{
public:
    VisaError() : _exception() {}
    VisaError(std::string exp) : _exception(exp) {}
    ~VisaError() throw () {}
    const std::string toString() const throw() { return _exception; }
    const char* what() {
        return "Visa Error";
    }
private:
    std::string _exception;
};

class VisaTimeoutSetError : public VisaError
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

class VisaConnectionError : public VisaError
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