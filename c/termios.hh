#include <termios.h>

#ifndef C_TERMIOS_HH
#define C_TERMIOS_HH

namespace std
{
    extern "C" {
        int cfsetspeed(struct termios *, speed_t);
        int tcgetattr(int, struct termios *);
        int tcsetattr(int, int, const struct termios *);
    }
}

#endif
