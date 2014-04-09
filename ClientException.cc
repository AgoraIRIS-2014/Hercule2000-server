#include <cstdint>
#include "ClientException.h"

ClientException::ClientException(int32_t number, int32_t nerror) throw()
     : InetSocketException(number, nerror)
{
}
