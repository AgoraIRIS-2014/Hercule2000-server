#include <sys/stat.h>

#ifndef C_SYS_STAT_H
#define C_SYS_STAT_H

namespace std
{
     extern "C" {
          int mkdir(const char *, mode_t);
     }
}

#endif
