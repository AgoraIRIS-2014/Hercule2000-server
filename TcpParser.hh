#include "Client.hh"
#include "Parser.hh"

#ifndef TCPPARSER_HH
#define TCPPARSER_HH

#define HP "HP"

class TcpParser : public Parser {
public:
     TcpParser(Client& cli, const char *data);
     
     virtual void parse();
     void hello(Client& cli);
     void setMaster(std::string id, Client& cli);

private:
     Client *cli_;
};

#endif
