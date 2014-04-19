#include <cstdint>
#include <string>
#include "InetSocketException.hh"

#ifndef NETWORKEXCEPTION_HH
#define NETWORKEXCEPTION_HH

class NetworkException : public InetSocketException {
public:
     NetworkException(std::string name, int32_t error) throw();
};

#endif
