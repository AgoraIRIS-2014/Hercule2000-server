#include <cstdint>
#include "ClientException.hh"

ClientException::ClientException(int32_t number, int32_t nerror) throw()
     : InetSocketException(number, nerror)
{
}
