#include <cstdint>
#include <string>
#include "InetSocketException.hh"

InetSocketException::InetSocketException(std::string name, int32_t error) throw()
     : SocketException(name, error) {}
