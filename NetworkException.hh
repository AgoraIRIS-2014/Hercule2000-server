#include <cstdint>
#include "InetSocketException.hh"

#ifndef NETWORKEXCEPTION_HH
#define NETWORKEXCEPTION_HH

class NetworkException : public InetSocketException {
public:
     NetworkException(int32_t number, int32_t nerror) throw();
};

#endif
