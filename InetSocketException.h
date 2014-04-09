#include <cstdint>
#include "SocketException.h"

#ifndef INETSOCKETEXCEPTION_H
#define INETSOCKETEXCEPTION_H

class InetSocketException : public SocketException {
public:
     InetSocketException(int32_t number, int32_t nerror) throw();
};

#endif
