#include <cerrno>
#include <string>
#include "config.hh"
#include "env.hh"
#include "Client.hh"
#include "ClientException.hh"
#include "TcpParser.hh"
#include "TcpParserException.hh"

#include <iostream> // debug
#include <thread> // debug

TcpParser::TcpParser(Client& cli, const char *data) : Parser(data), cli_(&cli) {}

void
TcpParser::parse()
{
     int8_t mode = cli_->getMode();

     if (data_ != "ALIVE") {
          try {
               if (mode < MODE_N) {

                    if (data_ == "HELLO")
                         helloRep(*cli_);
                    else if (!data_.compare(0, 3, "ID:")) {
                         setMaster(data_.substr(3, std::string::npos), *cli_);
                         cli_->setMode(MODE_N);
                    } else
                         throw TcpParserException("parse", EINVAL);

               } else {

                    if (data_ == "MODE:A")
                         cli_->setMode(MODE_A);
                    else if (data_ == "MODE:L")
                         cli_->setMode(MODE_L);
                    else if (data_ == "MODE:M")
                         cli_->setMode(MODE_M);
                    else if (data_ == "GET:LIST")
                         cli_->sendList();
                    else if (mode == MODE_A);
                    else if (mode == MODE_L) {

                         if (!data_.compare(0, 9, "FILENAME:"))
                              cli_->createTmpFile(data_.substr(9, std::string::npos));
                         else if (data_ == "CHECKPOINT")
                              cli_->makeFile();
                         else if (data_ == "EOF") {
                              cli_->makeFile();
                              cli_->deleteTmpFile();
                              env::posinit();
                              env::flag = 1;
                         }

                    } else
                         throw TcpParserException("parse", EINVAL);
               }
          } catch (const ClientException& e) {
               throw TcpParserException("parse", e.getError());
          }
     }
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
     cli.setID(id);

     if (env::client == NULL) {
          env::client = &cli;
          env::client->master();
     } else if (env::client->getID() == id ||
                (env::client->getID() == HP && id != HP)) {
          // Placer en WAIT le new client
          cli.wait();
          env::cliQueue.push_front(&cli);
     } else if (env::client->getID() != HP && id == HP) {
          // Placer en WAIT le client et donner la main au new client
          env::client->wait();
          env::cliQueue.push_front(env::client);
          env::client = &cli;
          env::client->master();
     }
}
