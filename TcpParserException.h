#include <cstdint>
#include "ParserException.h"

#ifndef TCPPARSEREXCEPTION_H
#define TCPPARSEREXCEPTION_H

class TcpParserException : public ParserException {
public:
     TcpParserException(int32_t number, int32_t nerror) throw();
};

#endif
