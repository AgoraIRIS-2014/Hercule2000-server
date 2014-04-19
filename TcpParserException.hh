#include <cstdint>
#include <string>
#include "ParserException.hh"

#ifndef TCPPARSEREXCEPTION_HH
#define TCPPARSEREXCEPTION_HH

class TcpParserException : public ParserException {
public:
     TcpParserException(std::string name, int32_t error) throw();
};

#endif
