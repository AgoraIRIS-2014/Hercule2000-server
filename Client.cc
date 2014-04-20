#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include "c/dirent.hh"
#include "config.hh"
#include "env.hh"
#include "SocketException.hh"
#include "Client.hh"
#include "ClientException.hh"
#include "TcpParser.hh"
#include "TcpParserException.hh"

#include <thread> // debug

Client::Client(int32_t fd) : InetSocket(fd), mode_(NOMODE) {}

Client::~Client()
{
     deleteFile();
}

void
Client::createFile(std::string filename)
{
     std::string path = config::dir + filename + ".h2kc";

     file_.open(path);
     
     if (!file_.fail()) {
          file_.close();
          send("FILEEXIST\n", sizeof "FILEEXIST\n");
          throw ClientException("createTmpFile", EEXIST);
     }

     path = config::tmpdir + filename + ".h2kc";

     file_.open(path);

     if (!file_.fail()) {
          file_.close();
          send("FILEEXIST\n", sizeof "FILEEXIST\n");
          throw ClientException("createTmpFile", EEXIST);
     }

     filename_ = filename + ".h2kc";
     file_.open(path, std::ios_base::in | std::ios_base::out |
                std::ios_base::trunc);
     send("OK\n", sizeof "OK\n");
}

void
Client::deleteFile()
{
     if (file_.is_open()) {
          file_.close();

          if (filename_.size() > 0) {
               std::string cmd = "rm -f " + config::tmpdir + filename_;
               std::system(cmd.data());
               filename_.clear();
          }
     }
}

bool
Client::fileIsOpen()
{
     if (file_.is_open())
          return true;
     
     return false;
}

std::string
Client::getID()
{
     return id_;
}

int8_t
Client::getMode()
{
     return mode_;
}

void
Client::master()
{
     send(MASTER, sizeof MASTER);
}

void
Client::readFile(std::string filename)
{
     bool flag;

     filename = config::dir + filename + ".h2kc";

     file_.open(filename);
     
     if (!fileIsOpen()) {
          send("FILEEXIST\n", sizeof "FILEEXIST\n");
          throw ClientException("readFile", ENOENT);
     }

     send("OK\n", sizeof "OK\n");

     flag = true;
     while (!file_.eof()) {
          if (env::mtx.try_lock() || flag) {
               if (flag)
                    flag = false;

               if (env::flag == 0) {
                    file_ >> env::move;
                    env::move.push_back('\n');
                    env::flag = 1;
               }
               env::mtx.unlock();
          }
     }

     env::mtx.lock();
     env::move = POSINIT;
     env::flag = 1;
     env::mtx.unlock();

     file_.close();
}

void
Client::saveFile()
{
     if (!fileIsOpen())
          throw ClientException("saveFile", ENOENT);

     file_.flush();

     std::string cmd = "cp -f " + config::tmpdir + filename_ + " "
          + config::dir + filename_;

     std::system(cmd.data());
}

void
Client::sendList()
{
     std::string list;
     struct dirent *dp;
     DIR *dir;

     dir = std::opendir(config::dir.data());

     if (dir == NULL)
          throw ClientException("sendList", ENOENT);

     list = "LIST:";

     while ((dp = std::readdir(dir)) != NULL) {
          if (std::strcmp(dp->d_name, ".h2kc") > 0) {
               list += dp->d_name;
               list.replace(list.size()-5, 5, 1, ';');
          }
     } 

     if (list[list.size()-1] == ';')
          list[list.size()-1] = '\n';

     std::closedir(dir);
     
     send(list.data(), list.size());
}

void
Client::setID(std::string id)
{
     id_ = id;
}

void
Client::setMode(int8_t mode)
{
     mode_ = mode;
}

void
Client::wait()
{
     send(WAIT, sizeof WAIT);
}

void
Client::writeFile(std::string data)
{
     if (!fileIsOpen())
          throw ClientException("writeFile", ENOENT);

     file_ << data;
}

void
Client::thread(int32_t fd, uint32_t rAddr, bool *alive)
{
     std::list<Client *>::iterator it;
     int32_t poll;
     char buf[NET_BUFLEN];

     Client cli(fd);
     cli.setRemoteAddr(rAddr);
      
     try {
          for (;;) {
               poll = cli.pollin(TCP_TIMEOUT * 1000);

               if (!poll)
                    throw ClientException("thread", ETIMEDOUT);

               cli.recv(buf, sizeof buf - 1);

               env::mtx.lock();
               //std::cout << "Client::thread lock 1" << std::endl; // debug
               try {
                    TcpParser parser(cli, buf);

                    parser.check();
                    parser.parse();
               } catch (const ParserException& e) {
                    std::cerr << e.what() << std::endl;
               }
               env::mtx.unlock();
               //std::cout << "Client::thread unlock 1" << std::endl; // debug

          }
     } catch (const SocketException& e) {
          std::cerr << e.what() << std::endl;
     }

     env::mtx.lock();
     //std::cout << "Client::thread lock 2" << std::endl; // debug
     if (&cli == env::client) {
          env::client = NULL;
          env::posinit();
          env::flag = 1;
     } else {
          it = env::cliQueue.begin();
          while (it != env::cliQueue.end()) {
               if (*it == &cli)
                    it = env::cliQueue.erase(it);
               else
                    it++;
          }
     }
     env::mtx.unlock();
     //std::cout << "Client::thread unlock 2" << std::endl; // debug

     *alive = false;
}
