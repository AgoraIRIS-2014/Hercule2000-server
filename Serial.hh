#include <cstddef>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include "c/termios.hh"

#ifndef SERIAL_HH
#define SERIAL_HH

class Serial {
public:
     Serial(std::string dev, const struct termios& serialConf);
     ~Serial();

     ssize_t read(void *buf, size_t nbyte);
     ssize_t send(const void *buf, size_t nbyte);                       

private:
     struct termios sysSerialConf_;
     int32_t fd_;
};

#endif
