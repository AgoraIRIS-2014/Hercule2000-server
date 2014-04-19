#include <cstdint>
#include <string>
#include "ParserException.hh"

#ifndef UDPPARSEREXCEPTION_HH
#define UDPPARSEREXCEPTION_HH

class UdpParserException : public ParserException {
public:
     UdpParserException(std::string name, int32_t error) throw();
};

#endif
