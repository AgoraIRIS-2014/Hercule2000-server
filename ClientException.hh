#include <cstdint>
#include "InetSocketException.hh"

#ifndef CLIENTEXCEPTION_HH
#define CLIENTEXCEPTION_HH

class ClientException : public InetSocketException {
public:
     ClientException(int32_t number, int32_t nerror) throw();
};

#endif
