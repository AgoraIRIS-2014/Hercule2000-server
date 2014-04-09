#include <cstring>
#include <chrono>
#include <string>
#include <thread>
#include <sys/types.h>
#include "c/termios.h"
#include "config.h"
#include "env.h"
#include "Robot.h"

#include <iostream> // debug

Robot::Robot(const struct termios serialConf)
     : Serial(config::serialDev, serialConf)
{
}

Robot::~Robot()
{
}

void
Robot::init(char mode)
{
     char rd[11];
     char r = 'r';

     //send(&r, 1);
     //read(&rd, 11);
     //send(&mode, 1);
     std::this_thread::sleep_for(std::chrono::seconds(1));

     env::posAxes.b = 0;
     env::posAxes.c = 0;
     env::posAxes.e = 0;
     env::posAxes.p = 0;
     env::posAxes.r = 0;
     env::posAxes.t = 0;

     env::flag = 0;

     // Positionnement du robot en position initial
     //move(POSINIT);
}

void
Robot::move(std::string cmd)
{
     char buf[2];

     send(cmd.data(), cmd.size());
     read(buf, sizeof buf);
}

ssize_t
Robot::read(void *buf, size_t nbyte)
{
     ssize_t read;
     size_t len;
     char *pbuf;

     len = nbyte;
     pbuf = (char *) buf;

     while (len > 0) {
          read = Serial::read(pbuf, len);
          len -= read;
          pbuf += read;
     }

     return nbyte - len;
}

ssize_t
Robot::send(const void *buf, size_t nbyte)
{
     ssize_t send;
     size_t len;
     char *pbuf;

     len = nbyte;
     pbuf = (char *) buf;

     while (len > 0) {
          send = Serial::send(pbuf, len);
          len -= send;
          pbuf += send;
     }

     return nbyte - len;
}

void
Robot::thread()
{
     struct termios serialConf;
    
     // Configuration du port série
     serialConf.c_iflag = 0;
     serialConf.c_oflag = 0;
     serialConf.c_cflag = CS8 | CREAD | CLOCAL;
     serialConf.c_lflag = 0;
     // Aucun caractère de contrôle
     std::memset(serialConf.c_cc, 0, NCCS);
     // Vitesse régler à 9600 bauds en entrée et sortie 
     std::cfsetspeed(&serialConf, B9600);
    
     Robot Hercule(serialConf);
     Hercule.init('L');
     std::this_thread::sleep_for(std::chrono::seconds(5));

     for (;;) {
          env::mtx.lock();
          if (env::flag == 2) {
               env::flag = 1;

               //move(env::move);
               env::move.clear();

               env::flag = 0;
          }
          env::mtx.unlock();
       }
}
