#include <cstdint>
#include <string>
#include "UdpParserException.hh"

UdpParserException::UdpParserException(std::string name, int32_t error) throw()
     : ParserException(name, error) {}
