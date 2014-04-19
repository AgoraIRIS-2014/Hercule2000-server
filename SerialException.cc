#include <cstdint>
#include <cstring>
#include <string>
#include "SerialException.hh"

SerialException::SerialException(std::string name, int32_t error) throw()
     : name_(name), error_(error) {}

const char *
SerialException::what() const throw()
{
     return std::strerror(error_);
}

int32_t
SerialException::getError() const throw()
{
     return error_;
}

std::string
SerialException::getName() const throw()
{
     return name_;
}
