#include <cstdint>
#include <exception>
#include <string>

#ifndef SERIALEXCEPTION_HH
#define SERIALEXCEPTION_HH

class SerialException : public std::exception {
public:
     SerialException(std::string name, int32_t error) throw();

     virtual const char *what() const throw();
     int32_t getError() const throw();
     std::string getName() const throw();

private:
     std::string name_;
     int32_t error_;
};

#endif
