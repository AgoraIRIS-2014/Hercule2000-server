#include <cstdint>
#include "UdpParserException.hh"

UdpParserException::UdpParserException(int32_t number, int32_t nerror) throw()
     : ParserException(number, nerror)
{
}
