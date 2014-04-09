#include <sys/select.h>

#ifndef C_SYS_SELECT_H
#define C_SYS_SELECT_H

namespace std
{
     extern "C" {
          int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
     }
}

#endif
