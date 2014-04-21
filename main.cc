#include <cstdint>
#include <cstdlib>
#include <list>
#include <string>
#include <thread>
#include "config.hh"
#include "env.hh"
#include "Client.hh"
#include "Network.hh"
#include "Robot.hh"

std::string config::dir = "/tmp/hercule-final/";
std::string config::serialDev = "/dev/ttyS0";
std::string config::tcpAddr = "*";
std::string config::tmpdir = "/tmp/hercule/";
std::string config::udpAddr = "*";
uint16_t config::maxClients = 3;
uint16_t config::tcpPort = 4242;
uint16_t config::udpPort = 42117;

std::string env::move;
std::list<Client *> env::cliQueue; 
Client *env::client = NULL;
std::mutex env::mtx;
struct botaxes env::posAxes;
struct botaxes env::mvAxes[2];
int8_t env::flag = -1;
int8_t env::run = 0;

int32_t
main()
{
     std::string cmd;

     env::cliQueue.push_front(env::client);

     cmd = "mkdir -p " + config::dir;
     std::system(cmd.data());
     cmd = "mkdir -p " + config::tmpdir;
     std::system(cmd.data());
     cmd = "rm -f " + config::tmpdir + "*.h2kc";
     std::system(cmd.data());
 
     std::thread robot(Robot::thread);
     std::thread netTCP(Network::tcpThread);
     std::thread netUDP(Network::udpThread);

     robot.join();
     netTCP.join();
     netUDP.join();

     return EXIT_SUCCESS;
}
