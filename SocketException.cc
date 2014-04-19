#include <cstdint>
#include <cstring>
#include <string>
#include "SocketException.hh"

SocketException::SocketException(std::string name, int32_t error) throw()
     :  name_(name), error_(error)
{
}

const char *
SocketException::what() const throw()
{
     return std::strerror(error_);
}

std::string
SocketException::getName() const throw()
{
     return name_;
}

int32_t
SocketException::getError() const throw()
{
     return error_;
}
