#include <cstdint>
#include <list>
#include <mutex>
#include <string>
#include "botaxes.h"
#include "Client.h"

#ifndef ENV_H
#define ENV_H

struct env {
     static std::list<Client *> cliQueue;
     static std::string move;
     static std::mutex mtx;
     static Client *client;
     static struct botaxes posAxes;
     static struct botaxes mvAxes[2];
     static int8_t flag;
};

#endif
