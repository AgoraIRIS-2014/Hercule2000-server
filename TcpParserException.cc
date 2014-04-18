#include <cstdint>
#include "TcpParserException.hh"

TcpParserException::TcpParserException(int32_t number, int32_t nerror) throw()
     : ParserException(number, nerror)
{
}
