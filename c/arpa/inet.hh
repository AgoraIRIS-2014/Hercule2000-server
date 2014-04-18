#include <arpa/inet.h>

#ifndef C_ARPA_INET_HH
#define C_ARPA_INET_HH

namespace std
{
     extern "C" {
          char *inet_ntoa(struct in_addr);
     }
}

#endif
