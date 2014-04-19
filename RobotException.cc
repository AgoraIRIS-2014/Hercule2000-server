#include <cstdint>
#include <string>
#include "RobotException.hh"

RobotException::RobotException(std::string name, int32_t error) throw()
     : SerialException(name, error) {}
