#include <cstdint>
#include "InetSocketException.h"

#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

class NetworkException : public InetSocketException {
public:
     NetworkException(int32_t number, int32_t nerror) throw();
};

#endif
