#include <cstdint>
#include "InetSocketException.hh"

InetSocketException::InetSocketException(int32_t number, int32_t nerror) throw()
     : SocketException(number, nerror)
{
}
