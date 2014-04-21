#include <cstdint>
#include <string>

#ifndef CONFIG_HH
#define CONFIG_HH

struct config {
     static std::string dir;
     static std::string serialDev;
     static std::string tcpAddr;
     static std::string tmpdir;
     static std::string udpAddr;
     static uint16_t maxClients;
     static uint16_t tcpPort;
     static uint16_t udpPort;
};

#endif
