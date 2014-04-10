#include <cerrno>
#include <string>
#include "config.h"
#include "env.h"
#include "Client.h"
#include "TcpParser.h"
#include "TcpParserException.h"

#include <iostream> // debug

TcpParser::TcpParser(Client& cli, const char *data) : Parser(data), cli_(&cli)
{
}

void
TcpParser::parse()
{
     if (data_ == "HELLO") {
          helloRep(*cli_);
     } else if (!data_.compare(0, 3, "ID:"))
          setMaster(data_.substr(3, std::string::npos), *cli_);
     else if (data_ == "MODE:A")
          cli_->setMode(MODE_A);
     else if (data_ == "MODE:L")
          cli_->setMode(MODE_L);
     else if (data_ == "MODE:M") {
          cli_->setMode(MODE_M);
          std::cout << "MODE:M" << std::endl; // debug
     } else if (data_ == "GET:LIST") {
          std::string a = "LIST:aaaaaaaaa;bbbbbbbbbbb;cccccccccccc;dddddddddd;eeeeeeeeeeee;fffffffffff;ggggggggggggggg;hhhhhhhhhhhhh;Taaaaaaaa;Tbbbbbbbbbb;Tccccccccccc;Tddddddddd;Teeeeeeeeeee;Tffffffffff;Tgggggggggggggg;Thhhhhhhhhhhh;Eaaaaaaaa;Ebbbbbbbbbb;Eccccccccccc;Eddddddddd;Eeeeeeeeeeee;Effffffffff;Egggggggggggggg;EhhhhhhhhhhhhEOF\n";
          cli_->send(a.data(), a.size());
     } else if (data_ != "ALIVE")
          throw TcpParserException(1, EINVAL);
}

void
TcpParser::helloRep(Client& cli)
{
     std::string UdpAddr, UdpPort;

     cli.getLocalAddr(UdpAddr);

     UdpPort = std::to_string(config::udpPort);
     UdpPort.push_back('\n');

     cli.send(UdpAddr.data(), UdpAddr.size());
     cli.send(UdpPort.data(), UdpPort.size());
}

void
TcpParser::setMaster(std::string id, Client& cli)
{
     std::cout << id << std::endl; // debug
     cli.setID(id);

     if (env::client == NULL) {
          env::client = &cli;
          env::client->master();
          std::cout << "1" << std::endl; // debug
     } else if (env::client->getID() == id ||
                (env::client->getID() == HP && id != HP)) {
          // Placer en WAIT le new client
          cli.wait();
          env::cliQueue.push_front(&cli);
          std::cout << "2" << std::endl; // debug
     } else if (env::client->getID() != HP && id == HP) {
          // Placer en WAIT le client et donner la main au new client
          env::client->wait();
          env::cliQueue.push_front(env::client);
          env::client = &cli;
          env::client->master();
          std::cout << "3" << std::endl; // debug
     }
}
