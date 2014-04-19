#include <cstring>
#include "env.hh"

void
env::posinit()
{
     std::memset(&env::posAxes, 0, sizeof env::posAxes);
     std::memset(&env::mvAxes[0], 0, sizeof env::mvAxes[0]);
     env::move = POSINIT;
}
