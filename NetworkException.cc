#include <cstdint>
#include "NetworkException.hh"

NetworkException::NetworkException(std::string name, int32_t error) throw()
     : InetSocketException(name, error) {}
