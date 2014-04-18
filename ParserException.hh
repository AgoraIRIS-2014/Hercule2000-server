#include <cstdint>
#include <exception>

#ifndef PARSEREXCEPTION_HH
#define PARSEREXCEPTION_HH

class ParserException : public std::exception {
public:
     ParserException(int32_t number, int32_t nerror) throw();

     virtual const char *what() const throw();
     int32_t getNerror() const throw();
     int32_t getNumber() const throw();

private:
     int32_t nerror_;
     int32_t number_;
};

#endif
