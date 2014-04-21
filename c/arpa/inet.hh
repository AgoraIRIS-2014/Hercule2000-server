#include <arpa/inet.h>

#ifndef C_ARPA_INET_HH
#define C_ARPA_INET_HH

namespace std
{
     extern "C" {
          in_addr_t inet_addr(const char *cp);
          char *inet_ntoa(struct in_addr);
     }
}

#endif
