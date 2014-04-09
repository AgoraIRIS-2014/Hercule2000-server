#include <poll.h>

#ifndef C_POLL_H
#define C_POLL_H

namespace std
{
    extern "C" {
        int poll(struct pollfd *, nfds_t, int);        
    }
}

#endif
