#include <cstdint>
#include <string>
#include "ClientException.hh"

ClientException::ClientException(std::string name, int32_t error) throw()
     : InetSocketException(name, error) {}
