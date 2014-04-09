#include <cstdint>
#include <exception>

#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

class SocketException : public std::exception {
public:
     SocketException(int32_t number, int32_t nerror) throw();

     virtual const char *what() const throw();
     int32_t getNerror() const throw();
     int32_t getNumber() const throw();

private:
     int32_t nerror_;
     int32_t number_;
};

#endif
