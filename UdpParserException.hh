#include <cstdint>
#include "ParserException.hh"

#ifndef UDPPARSEREXCEPTION_HH
#define UDPPARSEREXCEPTION_HH

class UdpParserException : public ParserException {
public:
     UdpParserException(int32_t number, int32_t nerror) throw();
};

#endif
