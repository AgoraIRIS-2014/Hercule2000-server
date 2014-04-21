#include <cstdint>
#include <list>
#include <mutex>
#include <string>
#include "botaxes.hh"
#include "Client.hh"

#ifndef ENV_HH
#define ENV_HH

#define POSINIT "B+0:30E+0:30C+0:30T+0:30R+0:30P-511:8\n" 

struct env {
     static std::list<Client *> cliQueue;
     static std::string move;
     static std::mutex mtx;
     static Client *client;
     static struct botaxes posAxes;
     static struct botaxes mvAxes[2];
     static int8_t flag;
     static int8_t run;

     static void posinit();
};

#endif
