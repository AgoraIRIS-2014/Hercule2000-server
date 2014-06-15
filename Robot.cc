#include <cstring>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <sys/types.h>
#include "c/termios.hh"
#include "config.hh"
#include "env.hh"
#include "Client.hh"
#include "Robot.hh"
#include "RobotException.hh"

Robot::Robot(const struct termios serialConf)
     : Serial(config::serialDev, serialConf) {}

void
Robot::init()
{
     char rd[11];
     char r = 'r';
     char l = 'L';

     send(&r, 1);
     read(&rd, 11);
     send(&l, 1);
     std::this_thread::sleep_for(std::chrono::seconds(1));

     env::posinit();
     move();
     env::flag = 0;
}

void
Robot::move()
{
     int16_t buf;

     if (env::move[0] != '\n') {
          if (env::client != NULL && env::client->getMode() == MODE_L &&
              !env::client->fileIsOpen())
               throw RobotException("move", ENOENT);

          send(env::move.data(), env::move.size());
          read(&buf, sizeof buf);

          if (buf == 0x4300) {
               if ((env::posAxes.b += env::mvAxes[0].b) < -511)
                    env::posAxes.b = -511;
               else if (env::posAxes.b > 511)
                    env::posAxes.b = 511 ;

               if ((env::posAxes.c += env::mvAxes[0].c) < -511)
                    env::posAxes.c = -511;
               else if (env::posAxes.c > 511)
                    env::posAxes.c = 511 ;

               if ((env::posAxes.e += env::mvAxes[0].e) < -511)
                    env::posAxes.e = -511;
               else if (env::posAxes.e > 511)
                    env::posAxes.e = 511 ;

               if ((env::posAxes.p += env::mvAxes[0].p) < -511)
                    env::posAxes.p = -511;
               else if (env::posAxes.p > 511)
                    env::posAxes.p = 511 ;

               if ((env::posAxes.r += env::mvAxes[0].r) < -511)
                    env::posAxes.r = -511;
               else if (env::posAxes.r > 511)
                    env::posAxes.r = 511 ;

               if ((env::posAxes.t += env::mvAxes[0].t) < -511)
                    env::posAxes.t = -511;
               else if (env::posAxes.t > 511)
                    env::posAxes.t = 511;

               if (env::client != NULL && env::client->getMode() == MODE_L)
                    env::client->writeFile(env::move);
           }

          std::memset(&env::mvAxes, 0, sizeof env::mvAxes);

          env::move.clear();
     }
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
    
     serialConf.c_iflag = 0;
     serialConf.c_oflag = 0;
     serialConf.c_cflag = CS8 | CREAD | CLOCAL;
     serialConf.c_lflag = 0;
     std::memset(serialConf.c_cc, 0, NCCS);
     std::cfsetspeed(&serialConf, B9600);
    
     Robot Hercule(serialConf);
     env::mtx.lock();
     Hercule.init();
     env::mtx.unlock();

     for (;;) {
          env::mtx.lock();
          try {
               if (env::flag == 1) {
                    Hercule.move();
                    env::flag = 0;
               }
          } catch (const RobotException& e) {
               env::flag = 0;
               std::cerr << e.what() << std::endl;
          }
          env::mtx.unlock();
     }
}
