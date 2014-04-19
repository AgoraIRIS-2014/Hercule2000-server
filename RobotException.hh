#include <cstdint>
#include <string>
#include "SerialException.hh"

#ifndef ROBOTEXCEPTION_HH
#define ROBOTEXCEPTION_HH

class RobotException : public SerialException {
public:
     RobotException(std::string name, int32_t error) throw();
};

#endif
