#include <arpa/inet.h>

#ifndef C_ARPA_INET_H
#define C_ARPA_INET_H

namespace std
{
     extern "C" {
          char *inet_ntoa(struct in_addr);
     }
}

#endif
