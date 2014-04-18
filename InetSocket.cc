#include <cstdint>
#include <string>
#include <netinet/in.h>
#include "c/arpa/inet.hh"
#include "InetSocket.hh"

InetSocket::InetSocket(int32_t type, int32_t protocol)
     : Socket(PF_INET, type, protocol),
       remoteAddr_((sockaddr_in *) &rAddr_), localAddr_((sockaddr_in *) &lAddr_)
{
}

InetSocket::InetSocket(int32_t fd)
     : Socket(fd),
       remoteAddr_((sockaddr_in *) &rAddr_), localAddr_((sockaddr_in *) &lAddr_)
{
}

uint32_t
InetSocket::getLocalAddr()
{
     return localAddr_->sin_addr.s_addr;
}

uint32_t
InetSocket::getLocalAddr(std::string& addr)
{
     addr = std::inet_ntoa(localAddr_->sin_addr);
     addr.push_back('\n');

     return localAddr_->sin_addr.s_addr;
}

uint32_t
InetSocket::getRemoteAddr()
{
     return remoteAddr_->sin_addr.s_addr;
}

uint16_t
InetSocket::getLocalPort()
{
     return ntohs(localAddr_->sin_port);
}

void
InetSocket::setLocalAddr(uint32_t addr)
{
     localAddr_->sin_addr.s_addr = htonl(addr);
}

void
InetSocket::setLocalPort(uint16_t port)
{
     localAddr_->sin_port = htons(port);
}

void
InetSocket::setRemoteAddr(uint32_t addr)
{
     remoteAddr_->sin_addr.s_addr = addr;
}
