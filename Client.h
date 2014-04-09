#include <cstdint>
#include <string>
#include "InetSocket.h"

#ifndef CLIENT_H
#define CLIENT_H

#define MODE_A 1
#define MODE_L 2
#define MODE_M 3

#define MASTER "MASTER\n"
#define SLEEP "SLEEP\n"

#define NET_BUFLEN 64
#define TCP_TIMEOUT 60000

class Client : public InetSocket {
public:
     Client(int32_t fd);

     std::string getID();
     uint8_t getMode();
     void setID(std::string id);
     void setMode(uint8_t mode);
     void master();
     void sleep();

     static void thread(int32_t fd, uint32_t rAddr, bool *alive);

private:
     std::string id_;
     uint8_t mode_;
};

#endif
