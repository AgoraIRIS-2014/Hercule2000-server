#include <string>
#include "InetSocket.hh"

#ifndef PARSER_HH
#define PARSER_HH

class Parser {
public:
     Parser(const char *data);
     
     virtual void check();
     virtual void parse() = 0;

protected:
     std::string data_;
};

#endif
