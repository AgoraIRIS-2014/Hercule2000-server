#include <cstdint>
#include <string>

#ifndef CONFIG_H
#define CONFIG_H

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
