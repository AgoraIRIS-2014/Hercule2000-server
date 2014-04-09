#include <cstddef>
#include <cstdint>
#include <list> 
#include "InetSocket.h"

#ifndef NETWORK_H
#define NETWORK_H

#define NET_BUFLEN 64
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

class Network : public InetSocket {
public:
     Network(int32_t type, uint16_t port);

     virtual int32_t accept();
     virtual ssize_t recvfrom(void *buf, size_t buflen, int32_t flags = 0);

     void appendClient(int32_t fd);
     void close();

     static void tcpThread();
     static void udpThread();

private:
     std::list<bool *> client_;
};

#endif
