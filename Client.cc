#include <cerrno>
#include <cstdint>
#include <iostream>
#include <string>
#include <sys/types.h>
#include "config.h"
#include "env.h"
#include "SocketException.h"
#include "Client.h"
#include "ClientException.h"
#include "TcpParser.h"
#include "TcpParserException.h"

Client::Client(int32_t fd) : InetSocket(fd), mode_(0) {}

Client::~Client()
{
     deleteTmpFile();
}

void
Client::createTmpFile(std::string filename)
{
     std::string path = config::dir + filename + ".h2kc";

     tmpfile_.open(path);
     
     if (!tmpfile_.fail()) {
          tmpfile_.close();
          throw ClientException(2, EEXIST);
     }

     path = config::tmpdir + filename + ".h2kc";

     tmpfile_.open(path);

     if (!tmpfile_.fail()) {
          tmpfile_.close();
          throw ClientException(2, EEXIST);
     }

     filename_ = filename + ".h2kc";
     tmpfile_.open(path, std::ios_base::in | std::ios_base::out |
                std::ios_base::trunc);
}

void
Client::deleteTmpFile()
{
     if (tmpfile_.is_open())
          tmpfile_.close();

     if (filename_.size() > 0) {
          std::string cmd = "rm -f " + config::tmpdir + filename_;
          std::system(cmd.data());
     }
}

std::string
Client::getID()
{
     return id_;
}

uint8_t
Client::getMode()
{
     return mode_;
}

void
Client::master()
{
     send(MASTER, sizeof MASTER);
}

void
Client::makeFile()
{
     std::string cmd = "cp -f " + config::tmpdir + filename_ + " "
          + config::dir + filename_;

     std::cout << "makeFile : " << cmd << std::endl; // debug
     std::system(cmd.data());
}

void
Client::setID(std::string id)
{
     id_ = id;
}

void
Client::setMode(uint8_t mode)
{
     mode_ = mode;
}

void
Client::wait()
{
     send(WAIT, sizeof WAIT);
}

void
Client::writeTmpFile(std::string data)
{
     tmpfile_ << data;
}

void
Client::thread(int32_t fd, uint32_t rAddr, bool *alive)
{
     int32_t poll;
     char buf[NET_BUFLEN];

     Client cli(fd);
     cli.setRemoteAddr(rAddr);
      
     try {
          for (;;) {
               poll = cli.pollin(TCP_TIMEOUT * 1000);

               if (!poll)
                    throw ClientException(3, ETIMEDOUT);

               cli.recv(buf, sizeof buf - 1);

               try {
                    TcpParser parser(cli, buf);
                    parser.check();
                    env::mtx.lock();
                    //std::cout << "Client::thread lock" << std::endl; // debug
                    parser.parse();
                    //std::cout << "Client::thread unlock 1" << std::endl; // debug
                    env::mtx.unlock();
               } catch (const ParserException& e) {
                    std::cerr << e.what() << std::endl;
               }
          }
     } catch (const SocketException& e) {
          std::cerr << e.what() << std::endl;
     }

     
     env::mtx.lock();
     //std::cout << "Client::thread lock 2" << std::endl; // debug
     if (&cli == env::client)
          env::client = NULL;
     //std::cout << "Client::thread unlock 2" << std::endl; // debug
     env::mtx.unlock();

     *alive = false;
}
