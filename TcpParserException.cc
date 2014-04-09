#include <cstdint>
#include "TcpParserException.h"

TcpParserException::TcpParserException(int32_t number, int32_t nerror) throw()
     : ParserException(number, nerror)
{
}
