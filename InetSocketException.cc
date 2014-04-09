#include <cstdint>
#include "InetSocketException.h"

InetSocketException::InetSocketException(int32_t number, int32_t nerror) throw()
     : SocketException(number, nerror)
{
}
