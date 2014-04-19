#include <cstdint>
#include <string>
#include "InetSocketException.hh"

#ifndef CLIENTEXCEPTION_HH
#define CLIENTEXCEPTION_HH

class ClientException : public InetSocketException {
public:
     ClientException(std::string name, int32_t error) throw();
};

#endif
