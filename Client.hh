#include <cstdint>
#include <fstream>
#include <string>
#include "InetSocket.hh"

#ifndef CLIENT_HH
#define CLIENT_HH

#define NOMODE -1
#define MODE_N 0
#define MODE_A 1
#define MODE_L 2
#define MODE_M 3

#define MASTER "MASTER\n"
#define WAIT "WAIT\n"

#define NET_BUFLEN 64
#define TCP_TIMEOUT 60

class Client : public InetSocket {
public:
     Client(int32_t fd);
     ~Client();

     void createFile(std::string filename);
     void deleteFile();
     bool fileIsOpen();
     std::string getID();
     int8_t getMode();
     void master();
     void readFile(std::string filename);
     void saveFile();
     void sendList();
     void setID(std::string id);
     void setMode(int8_t mode);
     void wait();
     void writeFile(std::string data);

     static void thread(int32_t fd, uint32_t rAddr, bool *alive);

private:
     std::string filename_;
     std::fstream file_;
     std::string id_;
     int8_t mode_;
};

#endif
