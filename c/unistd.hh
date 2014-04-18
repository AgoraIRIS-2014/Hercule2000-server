#include <unistd.h>

#ifndef C_UNISTD_HH
#define C_UNISTD_HH

namespace std
{
     extern "C" {
          ssize_t read(int, void *, size_t);
          int rmdir(const char *);
          unsigned int sleep(unsigned int);
          ssize_t write(int fd, const void *buf, size_t nbytes);
     }
}

#endif
