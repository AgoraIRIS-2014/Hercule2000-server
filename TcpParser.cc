#include <cerrno>
#include <cstdlib>
#include <string>
#include <thread>
#include "config.hh"
#include "env.hh"
#include "Client.hh"
#include "ClientException.hh"
#include "TcpParser.hh"
#include "TcpParserException.hh"

TcpParser::TcpParser(Client& cli, const char *data) : Parser(data), cli_(&cli) {}

void
TcpParser::parse()
{
     int8_t mode = cli_->getMode();

     if (data_ != "ALIVE") {
          try {
               if (mode < MODE_N) {

                    if (data_ == "HELLO")
                         hello(*cli_);
                    else if (!data_.compare(0, 3, "ID:")) {
                         setMaster(data_.substr(3, std::string::npos), *cli_);
                         cli_->setMode(MODE_N);
                    } else
                         throw TcpParserException("parse", EINVAL);

               } else if (env::client == cli_) {

                    if (data_ == "MODE:A")
                         cli_->setMode(MODE_A);
                    else if (data_ == "MODE:L")
                         cli_->setMode(MODE_L);
                    else if (data_ == "MODE:M")
                         cli_->setMode(MODE_M);
                    else if (data_ == "GET:LIST")
                         cli_->sendList();
                    else if (mode >= MODE_A) {

                         if (!data_.compare(0, 4, "DEL:"))
                              cli_->deleteFile(
                                   data_.substr(4, std::string::npos));
                         else if (mode == MODE_A) {

                              if (!data_.compare(0, 4, "RUN:")
                                  && env::run < 1) {
                                   env::run = 1;
                                   std::thread run
                                        (Client::run, cli_,
                                         data_.substr(4, std::string::npos));
                                   run.detach();
                              } else if (data_ == "ABORT")
                                   env::run = 0;

                         } else if (mode > MODE_A) {

                              if (data_ == "POSINIT") {
                                   env::posinit();
                                   env::flag = 1;
                              } else if (mode == MODE_L) {

                                   if (!data_.compare(0, 8, "SETNAME:"))
                                        cli_->createFile(
                                             data_.substr(8, std::string::npos));
                                   else if (cli_->fileIsOpen()) {

                                        if (data_ == "CHECKPOINT:BACK")
                                             cli_->restoreFile();
                                        else if (data_ == "CHECKPOINT:DONE")
                                             cli_->checkpoint();
                                        else if (data_ == "EOF") {
                                             cli_->saveFile();
                                             cli_->deleteFile();
                                             env::posinit();
                                             env::flag = 1;
                                        }

                                   }
                              } else
                                   throw TcpParserException("parse", EINVAL);

                         } else
                              throw TcpParserException("parse", EINVAL);

                    } else
                         throw TcpParserException("parse", EINVAL);

               } else
                    throw TcpParserException("parse", EACCES);

          } catch (const ClientException& e) {
               throw TcpParserException("parse", e.getError());
          }
     }
}

void
TcpParser::hello(Client& cli)
{
     std::string udpAddr;
     std::string udpPort;

     if (config::udpAddr == "*")
          cli.getLocalAddr(udpAddr);
     else
          udpAddr = config::udpAddr;

     udpPort = std::to_string(config::udpPort);
     udpPort.push_back('\n');

     cli.send(udpAddr.data(), udpAddr.size());
     cli.send(udpPort.data(), udpPort.size());
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
          cli.wait();
          env::cliQueue.push_front(&cli);
     } else if (env::client->getID() != HP && id == HP) {
          env::client->wait();
          env::cliQueue.push_front(env::client);
          env::client = &cli;
          env::client->master();
     }
}
