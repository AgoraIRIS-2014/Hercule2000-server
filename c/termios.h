#include <termios.h>

#ifndef C_TERMIOS_H
#define C_TERMIOS_H

namespace std
{
    extern "C" {
        int cfsetspeed(struct termios *, speed_t);
        int tcgetattr(int, struct termios *);
        int tcsetattr(int, int, const struct termios *);
    }
}

#endif
