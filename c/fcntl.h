#include <fcntl.h>

#ifndef C_FCNTL_H
#define C_FCNTL_H

namespace std
{
    extern "C" {
        int close(int);
        int open(const char *, int);
    }
}

#endif
