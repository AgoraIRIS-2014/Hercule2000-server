#include <cstddef>
#include <string>
#include <sys/types.h>
#include "c/fcntl.h"
#include "c/termios.h"
#include "c/unistd.h"
#include "Serial.h"

#include <iostream> //debug

Serial::Serial(std::string dev, const struct termios& serialConf)
{
     // Ouverture du fichier associé au port série
     //fd_ = std::open(dev.data(), O_RDWR | O_NOCTTY);
     //std::cout << "fd_ = " << fd_ << std::endl; // debug
     // Sauvegarde des paramètres initiaux du port série
     //std::tcgetattr(fd_, &sysSerialConf_);
     //
     //std::tcsetattr(fd_, TCSANOW, &serialConf);
}

Serial::~Serial()
{
     // Restauration des paramètres initiaux du port série
     std::tcsetattr(fd_, TCSANOW, &sysSerialConf_);
     // Fermeture du fichier associé au port série
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
