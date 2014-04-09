#include <cstdint>
#include <cstring>
#include "ParserException.h"

ParserException::ParserException(int32_t number, int32_t nerror) throw()
     : nerror_(nerror), number_(number)
{
}

const char *
ParserException::what() const throw()
{
     return std::strerror(nerror_);
}

int32_t
ParserException::getNerror() const throw()
{
     return nerror_;
}

int32_t
ParserException::getNumber() const throw()
{
     return number_;
}
