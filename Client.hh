#include <cstdint>
#include <fstream>
#include <string>
#include "InetSocket.hh"

#ifndef CLIENT_HH
#define CLIENT_HH

#define MODE_A 1
#define MODE_L 2
#define MODE_M 3

#define MASTER "MASTER\n"
#define WAIT "WAIT\n"

#define NET_BUFLEN 64
#define TCP_TIMEOUT 10

class Client : public InetSocket {
public:
     Client(int32_t fd);
     ~Client();

     void createTmpFile(std::string filename);
     void deleteTmpFile();
     std::string getID();
     uint8_t getMode();
     void makeFile();
     void master();
     void setID(std::string id);
     void setMode(uint8_t mode);
     void wait();
     void writeTmpFile(std::string data);

     static void thread(int32_t fd, uint32_t rAddr, bool *alive);

private:
     std::string filename_;
     std::fstream tmpfile_;
     std::string id_;
     uint8_t mode_;
};

#endif
