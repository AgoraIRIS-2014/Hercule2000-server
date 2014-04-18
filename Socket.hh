#include <cstddef>
#include <cstdint>
#include "c/poll.hh"
#include "c/sys/socket.hh"

#ifndef SOCKET_HH
#define SOCKET_HH

class Socket {
public:
     Socket(int32_t domain, int32_t type, int32_t protocol);
     Socket(int32_t fd);
     virtual ~Socket() = 0;

     virtual ssize_t recv(void *buf, size_t buflen, int32_t flags = 0);
     virtual ssize_t recvfrom(void *buf, size_t buflen, int32_t flags = 0);
     virtual ssize_t send(const void *buf, size_t nbyte, int32_t flags = 0);
     virtual int32_t accept();
     virtual int32_t bind();
     virtual int32_t getFd();
     virtual int32_t listen(int32_t backlog = 0);
     virtual int32_t pollin(int32_t timeout = 0);
     
protected:
     struct sockaddr rAddr_;
     struct sockaddr lAddr_;

private:
     struct pollfd poll_;
};

#endif
