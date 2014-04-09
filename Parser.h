#include <string>
#include "InetSocket.h"

#ifndef PARSER_H
#define PARSER_H

class Parser {
public:
     Parser(const char *data);
     
     virtual void check();
     virtual void parse() = 0;

protected:
     std::string data_;
};

#endif
