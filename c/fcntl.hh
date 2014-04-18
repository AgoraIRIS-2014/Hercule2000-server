#include <fcntl.h>

#ifndef C_FCNTL_HH
#define C_FCNTL_HH

namespace std
{
    extern "C" {
        int close(int);
        int open(const char *, int);
    }
}

#endif
