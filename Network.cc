#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <sys/types.h>
#include "config.h"
#include "env.h"
#include "Client.h"
#include "Network.h"
#include "NetworkException.h"
#include "SocketException.h"
#include "UdpParser.h"
#include "UdpParserException.h"

Network::Network(int32_t type, uint16_t port) : InetSocket(type, 0)
{
     if (config::netAddr == "*")
          setLocalAddr(INADDR_ANY);

     setLocalPort(port);

     bind();
}

int32_t
Network::accept()
{
     int32_t poll;

     if (client_.size() < config::maxClients) {
          try {
               poll = pollin(5);
               
               if (!poll)
                    return poll;

               return InetSocket::accept();
          } catch (const SocketException& e) {
               std::cerr << e.what() << std::endl;
          }
     }

     return -1;
}

void
Network::appendClient(int32_t fd)
{
     if (fd > 0) {
          client_.push_back(new bool);
                 
          *client_.back() = true;
                                  
          std::thread client(Client::thread, fd, getRemoteAddr(),
                             client_.back());
          client.detach();
     }
}

void
Network::close()
{
     std::list<bool *>::iterator it;

     it = client_.begin();
     while (it != client_.end()) {
          if (!**it) {
               delete *it;
                   
               it = client_.erase(it);
          } else {
               it++;
          }
     }
}

ssize_t
Network::recvfrom(void *buf, size_t buflen, int32_t flags)
{
     buflen = InetSocket::recvfrom(buf, buflen, flags);

     if (env::client == NULL)
          throw NetworkException(5, EACCES);

     if (env::client->getMode() <= MODE_A)
          throw NetworkException(5, EACCES);

     if (remoteAddr_->sin_addr.s_addr != env::client->getRemoteAddr())
          throw NetworkException(5, EACCES);

     std::cout << "UDP: " << (char *) buf << std::endl; // debug
     return buflen;
}

void
Network::tcpThread()
{
     int32_t fd;

     Network net(TCP, config::tcpPort);

     net.listen();

     for (;;) {
          fd = net.accept();
          net.appendClient(fd);
          net.close();
     }
}

void
Network::udpThread()
{
     socklen_t buflen;
     char buf[NET_BUFLEN];
     bool out;
 
     Network net(UDP, config::udpPort);

     for (;;) {
          try {
               buflen = net.recvfrom(buf, sizeof buf);
               buf[buflen] = '\0';
               
               try {
                    out = false;
                    while (out == false) {
                         env::mtx.lock();
                         if (env::flag == 0) {
                              env::flag = 1;

                              UdpParser parser(buf);
                              parser.check();
                              parser.parse();

                              env::flag = 2;
                              out = true;
                         }
                         env::mtx.unlock();
                    }
               } catch (const ParserException& e) {
                    std::cerr << e.what() << std::endl;
               }
          } catch (const NetworkException& e) {
               std::cerr << e.what() << std::endl;
          }
     }
}
