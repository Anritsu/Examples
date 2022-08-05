#pragma once
//#include "vcruntime_exception.h"
#include <exception>
#include <string>
class CustomException :
	public std::exception
{
public:
	CustomException(std::string exp) : _exception(exp) {}
	~CustomException() throw () {}
	const std::string toString() const throw() { return _exception; }
private:
	std::string _exception;
};

