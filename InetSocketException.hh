#include <cstdint>
#include <string>
#include "SocketException.hh"

#ifndef INETSOCKETEXCEPTION_HH
#define INETSOCKETEXCEPTION_HH

class InetSocketException : public SocketException {
public:
     InetSocketException(std::string name, int32_t error) throw();
};

#endif
