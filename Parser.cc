#include <cerrno>
#include <string>
#include "InetSocket.hh"
#include "Parser.hh"
#include "ParserException.hh"

#include <iostream> // debug

Parser::Parser(const char *data) : data_(data) {}

void
Parser::check()
{
     size_t fcp;

     fcp = data_.find_first_of('\n');
               
     if (fcp == std::string::npos)
          throw ParserException("check", EINVAL);
     
     data_ = data_.substr(0, fcp);
     std::cout << data_ << std::endl; // debug
}
