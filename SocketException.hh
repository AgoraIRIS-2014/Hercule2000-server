#include <cstdint>
#include <string>
#include <exception>

#ifndef SOCKETEXCEPTION_HH
#define SOCKETEXCEPTION_HH

class SocketException : public std::exception {
public:
     SocketException(std::string name, int32_t error) throw();

     virtual const char *what() const throw();
     std::string getName() const throw();
     int32_t getError() const throw();

private:
     std::string name_;
     int32_t error_;
};

#endif
