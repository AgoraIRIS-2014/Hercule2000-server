#include <poll.h>

#ifndef C_POLL_HH
#define C_POLL_HH

namespace std
{
    extern "C" {
        int poll(struct pollfd *, nfds_t, int);        
    }
}

#endif
