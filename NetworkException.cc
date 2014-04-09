#include <cstdint>
#include "NetworkException.h"

NetworkException::NetworkException(int32_t number, int32_t nerror) throw()
     : InetSocketException(number, nerror)
{
}
