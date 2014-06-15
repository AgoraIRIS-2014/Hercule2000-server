#include <cstddef>
#include <string>
#include <sys/types.h>
#include "c/fcntl.hh"
#include "c/termios.hh"
#include "c/unistd.hh"
#include "Serial.hh"

Serial::Serial(std::string dev, const struct termios& serialConf)
{
     fd_ = std::open(dev.data(), O_RDWR | O_NOCTTY);
     std::tcgetattr(fd_, &sysSerialConf_);
     std::tcsetattr(fd_, TCSANOW, &serialConf);
}

Serial::~Serial()
{
     std::tcsetattr(fd_, TCSANOW, &sysSerialConf_);
     std::close(fd_);
}

ssize_t
Serial::read(void *buf, size_t nbyte)
{
     return std::read(fd_, buf, nbyte);
}

ssize_t
Serial::send(const void *buf, size_t nbyte)
{
     return std::write(fd_, buf, nbyte);
}
