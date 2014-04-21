#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>
#include "env.hh"
#include "UdpParser.hh"
#include "UdpParserException.hh"

UdpParser::UdpParser(const char *data) : Parser(data) {}

void
UdpParser::checkMove(char *move, char **posss)
{
     char *find;
     char *i;

     find = std::strpbrk(move, "BCEPRT");

     if (find != &move[0])
          throw UdpParserException("checkMove", EINVAL);

     find = std::strpbrk(&move[1], "+-");

     if (find != &move[1])
          throw UdpParserException("checkMove", EINVAL);

     *posss = std::strpbrk(&move[2], ":");

     if (*posss == &move[2] || *posss == NULL)
          throw UdpParserException("checkMove", EINVAL);

     for (i = &move[2]; i < *posss; i++) {
          find = std::strpbrk(i, "0123456789");

          if (find != i)
               throw UdpParserException("checkMove", EINVAL);
     }

     for (i = *posss + 1; i < &move[std::strlen(move)]; i++) {
          find = std::strpbrk(i, "0123456789");

          if (find != i)
               throw UdpParserException("checkMove", EINVAL);
     }
}

void
UdpParser::parse()
{
     size_t numc;
     int16_t div; 
     int16_t ipos; 
     int16_t isp;
     char *buf;
     char *cmd;
     char *posss;

     cmd = std::strtok((char *) data_.data(), ";");

     while (cmd != NULL) {
          checkMove(cmd, &posss);

          ipos = parseMove(cmd, posss);
          isp = parseSpeed(cmd, posss);

          div = ipos;
          numc = 0;
          do {
               div /= 10;
               numc++;
          } while (div != 0);

          env::move.push_back(cmd[0]);

          if (ipos < 0)
               buf = new char[numc+1];
          else {
               buf = new char[numc];
               env::move.push_back('+');
          }

          std::sprintf(buf, "%d", ipos);
          env::move.append(buf);
          delete buf;

          env::move.push_back(':');
          
          div = isp;
          numc = 0;
          do {
               div /= 10;
               numc++;
          } while (div != 0);

          buf = new char[numc];
          std::sprintf(buf, "%d", isp);
          env::move.append(buf);
          delete buf;

          cmd = std::strtok(NULL, ";");
     }

     env::move.push_back('\n');
}

int16_t
UdpParser::parseMove(char *move, char *posss)
{
     size_t numc;
     int16_t ipos;
     char *i;
     char *pos;

     numc = 0;
     for (i = &move[1]; i < posss; i++)
          numc++;

     pos = new char[numc];
     std::strncpy(pos, &move[1], numc);
     ipos = (int16_t) std::atoi(pos);
     delete pos;

     switch (move[0]) {
     case 'B':
          env::mvAxes[0].b = ipos;
          ipos = env::posAxes.b + ipos;
          break;
     case 'C':
          env::mvAxes[0].c = ipos;
          ipos = env::posAxes.c + ipos;
          break;
     case 'E':
          env::mvAxes[0].e = ipos;
          ipos = env::posAxes.e + ipos;
          break;
     case 'P':
          env::mvAxes[0].p = ipos;
          ipos = env::posAxes.p + ipos;
          break;
     case 'R':
          env::mvAxes[0].r = ipos;
          ipos = env::posAxes.r + ipos;
          break;
     case 'T':
          env::mvAxes[0].t = ipos;
          ipos = env::posAxes.t + ipos;
          break;
     }
 
     if (ipos > 511)
          ipos = 511;
     else if (ipos < -511)
          ipos = -511;

     return ipos;
}

uint8_t
UdpParser::parseSpeed(char *move, char *posss)
{
     size_t numc;
     uint8_t isp;
     char *i;
     char *sp;

     numc = 0;
     for (i = posss; i < &move[std::strlen(move)]; i++)
          numc++;

     sp = new char[numc];
     std::strncpy(sp, posss+1, numc);
     isp = (uint8_t) std::atoi(sp);
     delete sp;

     switch (move[0]) {
     case 'B':
          env::mvAxes[1].b = isp;
          break;
     case 'C':
          env::mvAxes[1].c = isp;
          break;
     case 'E':
          env::mvAxes[1].e = isp;
          break;
     case 'P':
          env::mvAxes[1].p = isp;
          break;
     case 'R':
          env::mvAxes[1].r = isp;
          break;
     case 'T':
          env::mvAxes[1].t = isp;
          break;
     }

     return isp;
}
