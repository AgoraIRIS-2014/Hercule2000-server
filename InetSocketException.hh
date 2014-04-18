#include <cstdint>
#include "SocketException.hh"

#ifndef INETSOCKETEXCEPTION_HH
#define INETSOCKETEXCEPTION_HH

class InetSocketException : public SocketException {
public:
     InetSocketException(int32_t number, int32_t nerror) throw();
};

#endif
