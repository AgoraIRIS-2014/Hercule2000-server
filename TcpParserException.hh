#include <cstdint>
#include "ParserException.hh"

#ifndef TCPPARSEREXCEPTION_HH
#define TCPPARSEREXCEPTION_HH

class TcpParserException : public ParserException {
public:
     TcpParserException(int32_t number, int32_t nerror) throw();
};

#endif
