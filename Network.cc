#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <sys/types.h>
#include "config.hh"
#include "env.hh"
#include "Client.hh"
#include "Network.hh"
#include "NetworkException.hh"
#include "SocketException.hh"
#include "UdpParser.hh"
#include "UdpParserException.hh"

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
     char buf[NET_BUFLEN];
 
     Network net(UDP, config::udpPort);

     for (;;) {
          try {
               net.recvfrom(buf, sizeof buf);

               env::mtx.lock();
               //std::cout << "Network::udpThread lock" << std::endl; // debug
               if (env::client == NULL)
                    throw NetworkException("udpThread", EACCES);

               if (env::client->getMode() <= MODE_A)
                    throw NetworkException("udpThread", EACCES);

               if (net.getRemoteAddr() != env::client->getRemoteAddr())
                    throw NetworkException("udpThread", EACCES);

               if (env::flag == 0) {
                    try {
                         UdpParser parser(buf);
                         parser.check();
                         parser.parse();
                         env::flag = 1;
                    } catch (const ParserException& e) {
                         std::cerr << e.what() << std::endl;
                    }
               }
               env::mtx.unlock();
               //std::cout << "Network::udpthread unlock" << std::endl; // debug
          } catch (const NetworkException& e) {
               env::mtx.unlock();
               //std::cout << "Network::udpthread unlock" << std::endl; // debug
               std::cerr << e.what() << std::endl;
          }
     }
}
