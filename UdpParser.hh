#include <string>
#include "Parser.hh"

#ifndef UDPPARSER_HH
#define UDPPARSER_HH

class UdpParser : public Parser {
public:
     UdpParser(const char *data);
     
     virtual void parse();

private:
     int16_t parseMove(char *move, char *posss);
     uint8_t parseSpeed(char *move, char *posss);
     void checkMove(char *move, char **posss);
};

#endif
