#include <cstdint>
#include "ParserException.h"

#ifndef UDPPARSEREXCEPTION_H
#define UDPPARSEREXCEPTION_H

class UdpParserException : public ParserException {
public:
     UdpParserException(int32_t number, int32_t nerror) throw();
};

#endif
