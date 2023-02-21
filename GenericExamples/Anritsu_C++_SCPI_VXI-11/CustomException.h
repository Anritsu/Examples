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