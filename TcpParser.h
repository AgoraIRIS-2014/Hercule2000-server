#include "Client.h"
#include "Parser.h"

#ifndef TCPPARSER_H
#define TCPPARSER_H

#define HP "HP"

class TcpParser : public Parser {
public:
     TcpParser(Client& cli, const char *data);
     
     virtual void parse();
     void helloRep(Client& cli);
     void setMaster(std::string id, Client& cli);

private:
     Client *cli_;
};

#endif
