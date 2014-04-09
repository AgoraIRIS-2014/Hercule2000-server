#include <unistd.h>

#ifndef C_UNISTD_H
#define C_UNISTD_H

namespace std
{
    extern "C" {
        ssize_t read(int, void *, size_t);
        ssize_t write(int fd, const void *buf, size_t nbytes);
        unsigned int sleep(unsigned int);
    }
}

#endif
