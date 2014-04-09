#include <string>
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include "config.h"
#include "env.h"
#include "SocketException.h"
#include "Client.h"
#include "ClientException.h"
#include "TcpParser.h"
#include "TcpParserException.h"

Client::Client(int32_t fd) : InetSocket(fd), mode_(0)
{
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
Client::master()
{
     send(MASTER, sizeof MASTER);
}

void
Client::sleep()
{
     send(SLEEP, sizeof SLEEP);
}

void
Client::thread(int32_t fd, uint32_t rAddr, bool *alive)
{
     ssize_t recv;
     int32_t poll;
     char buf[NET_BUFLEN];

     Client cli(fd);
     cli.setRemoteAddr(rAddr);
      
     try {
          for (;;) {
               poll = cli.pollin(TCP_TIMEOUT);

               if (!poll)
                    throw ClientException(3, ETIMEDOUT);

               recv = cli.recv(buf, sizeof buf - 1);
               buf[recv] = '\0';

               try {
                    TcpParser parser(cli, buf);
                    parser.check();
                    parser.parse();
               } catch (const ParserException& e) {
                    std::cerr << e.what() << std::endl;
               }
          }
     } catch (const SocketException& e) {
          std::cerr << e.what() << std::endl;
     }

     if (&cli == env::client)
          env::client = NULL;

     *alive = false;
}
