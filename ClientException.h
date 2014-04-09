#include <cstdint>
#include "InetSocketException.h"

#ifndef CLIENTEXCEPTION_H
#define CLIENTEXCEPTION_H

class ClientException : public InetSocketException {
public:
     ClientException(int32_t number, int32_t nerror) throw();
};

#endif
