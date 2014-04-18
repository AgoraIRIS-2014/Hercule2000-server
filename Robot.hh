#include <cstdint>
#include <string>
#include <sys/types.h>
#include "Serial.hh"

#ifndef ROBOT_HH
#define ROBOT_HH

#define ROB_BUFLEN 11
#define POSINIT "B+0:30E+0:30C+0:30T+0:30R+0:30P-511:8\n" 
#define TIMEOUT 15
#define WTIME 0

class Robot : public Serial {
public:
     Robot(const struct termios serialConf);
     ~Robot();

     ssize_t read(void *buf, size_t nbyte);
     ssize_t send(const void *buf, size_t nbyte);
     void init(char mode);
     void make();
     void move();

     static void thread();

private:
};

#endif
