#include <cstdint>
#include <cstring>
#include "SocketException.h"

SocketException::SocketException(int32_t number, int32_t nerror) throw()
     :  nerror_(nerror), number_(number)
{
}

const char *
SocketException::what() const throw()
{
     return std::strerror(nerror_);
}

int32_t
SocketException::getNerror() const throw()
{
     return nerror_;
}

int32_t
SocketException::getNumber() const throw()
{
     return number_;
}
