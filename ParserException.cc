#include <cstdint>
#include <cstring>
#include <string>
#include "ParserException.hh"

ParserException::ParserException(std::string name, int32_t error) throw()
     : name_(name), error_(error) {}

const char *
ParserException::what() const throw()
{
     return std::strerror(error_);
}

int32_t
ParserException::getError() const throw()
{
     return error_;
}

std::string
ParserException::getName() const throw()
{
     return name_;
}
