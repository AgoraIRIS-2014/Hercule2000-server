#include <sys/select.h>

#ifndef C_SYS_SELECT_HH
#define C_SYS_SELECT_HH

namespace std
{
     extern "C" {
          int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
     }
}

#endif
