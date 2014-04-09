#include <sys/socket.h>

#ifndef C_SYS_SOCKET_H
#define C_SYS_SOCKET_H

namespace std
{
     extern "C" {
          ssize_t recv(int, void *, size_t, int);
          ssize_t recvfrom(int, void *, size_t, int, struct sockaddr *,
                           socklen_t *);
          ssize_t send(int, const void *, size_t, int);
          int accept(int, struct sockaddr *, socklen_t *);
          int bind(int, const struct sockaddr *, socklen_t);
          int getsockname(int, struct sockaddr *, socklen_t *);
          int listen(int, int);
          int socket(int, int, int);
     }
}

#endif
