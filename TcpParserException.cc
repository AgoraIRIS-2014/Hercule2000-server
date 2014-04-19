#include <cstdint>
#include <string>
#include "TcpParserException.hh"

TcpParserException::TcpParserException(std::string name, int32_t error) throw()
     : ParserException(name, error) {}
