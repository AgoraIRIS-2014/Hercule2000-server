#include <sys/stat.h>

#ifndef C_SYS_STAT_HH
#define C_SYS_STAT_HH

namespace std
{
     extern "C" {
          int mkdir(const char *, mode_t);
     }
}

#endif
