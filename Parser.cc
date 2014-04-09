#include <cerrno>
#include <string>
#include "InetSocket.h"
#include "Parser.h"
#include "ParserException.h"

#include <iostream> // debug

Parser::Parser(const char *data) : data_(data)
{
}

void
Parser::check()
{
     size_t fcp;

     fcp = data_.find_first_of('\n');
               
     if (fcp == std::string::npos)
          throw ParserException(1, EINVAL);
     
     data_ = data_.substr(0, fcp);
}
