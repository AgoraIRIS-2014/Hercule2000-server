#include <cstdint>
#include <string>
#include <sys/types.h>
#include "Serial.hh"

#ifndef ROBOT_HH
#define ROBOT_HH

#define ROB_BUFLEN 11
#define TIMEOUT 15
#define WTIME 0

class Robot : public Serial {
public:
     Robot(const struct termios serialConf);
     ~Robot();

     void init();
     void make();
     void move();
     ssize_t read(void *buf, size_t nbyte);
     ssize_t send(const void *buf, size_t nbyte);

     static void thread();

private:
};

#endif
