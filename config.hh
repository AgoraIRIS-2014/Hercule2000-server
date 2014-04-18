#include <cstdint>
#include <string>

#ifndef CONFIG_HH
#define CONFIG_HH

struct config {
     static std::string camAddr;
     static std::string dir;
     static std::string netAddr;
     static std::string serialDev;
     static std::string tmpdir;
     static uint16_t maxClients;
     static uint16_t tcpPort;
     static uint16_t udpPort;
};

#endif
