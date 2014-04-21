#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <sys/types.h>
#include "c/dirent.hh"
#include "config.hh"
#include "env.hh"
#include "SocketException.hh"
#include "Client.hh"
#include "ClientException.hh"
#include "TcpParser.hh"
#include "TcpParserException.hh"

Client::Client(int32_t fd) : InetSocket(fd), mode_(NOMODE) {}

Client::~Client()
{
     deleteFile();
}

void
Client::checkpoint()
{
     char buf[4];

     sprintf(buf, "%d", env::posAxes.b);
     env::move = "B";     
     if (env::posAxes.b >= 0)
          env::move += '+';
     env::move += buf;
     env::move += ":28";

     sprintf(buf, "%d", env::posAxes.c);
     env::move += 'C';
     if (env::posAxes.b >= 0)
          env::move += '+';
     env::move += buf;
     env::move += ":28";

     sprintf(buf, "%d", env::posAxes.e);
     env::move += 'E';
     if (env::posAxes.b >= 0)
          env::move += '+';
     env::move += buf;
     env::move += ":28";

     sprintf(buf, "%d", env::posAxes.p);
     env::move += 'P';
     if (env::posAxes.p >= 0)
          env::move += '+';
     env::move += buf;
     env::move += ":8";

     sprintf(buf, "%d", env::posAxes.r);
     env::move += 'R';
     if (env::posAxes.b >= 0)
          env::move += '+';
     env::move += buf;
     env::move += ":28";

     sprintf(buf, "%d", env::posAxes.t);
     env::move += 'T';
     if (env::posAxes.b >= 0)
          env::move += '+';
     env::move += buf;
     env::move += ":28\n";

     writeFile(env::move);
     env::move.clear();
     saveFile();
}

void
Client::createFile(std::string filename)
{
     std::string path = config::tmpdir + filename + ".h2kc";

     env::posinit();
     env::flag = 1;

     filename_ = filename + ".h2kc";

     file_.open(path, std::ios_base::in | std::ios_base::out |
                std::ios_base::trunc);
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

void
Client::deleteFile(std::string filename)
{
     std::string cmd;
     filename += ".h2kc";

     if (filename == filename_)
          throw ClientException("deleteFile", EBUSY);

     if (filename.size() > 0) {
          cmd = "rm -f " + config::dir + filename;
          std::system(cmd.data());
          filename_.clear();
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
     std::string master = MASTER;
     std::streampos pos, bpos;
     char c;

     if (file_.is_open()) {
          file_.seekg(0, file_.beg);
          bpos = file_.tellg();

          file_.seekg(0, file_.end);
          file_.unget();
  
          do {
               file_.unget();
               file_.get(c);
               file_.unget();
               pos = file_.tellg();
          } while (c != '\n' && pos != bpos);
          
          file_ >> env::move;
          env::move += '\n';

          env::posAxes.b = axes.b;
          env::posAxes.e = axes.e;
          env::posAxes.c = axes.c;
          env::posAxes.t = axes.t;
          env::posAxes.r = axes.r;
          env::posAxes.p = axes.p;
 
          env::flag = 1;
     } else {
          env::posinit();
          env::flag = 1;
     }

     send(master.data(), master.size());
}

void
Client::readFile(std::string filename)
{
     bool flag;

     filename = config::dir + filename + ".h2kc";

     file_.open(filename);
     
     if (!file_.is_open())
          throw ClientException("readFile", ENOENT);
  
     flag = true;
     while (!file_.eof()) {
          if (env::mtx.try_lock() || flag) {
               if (env::client != this || env::run < 1) {
                    env::mtx.unlock();
                    break;
               }

               if (flag)
                    flag = false;

               if (env::flag == 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    file_ >> env::move;
                    env::move.push_back('\n');
                    env::flag = 1;
               }

               env::mtx.unlock();

               if (env::move.find('P') != std::string::npos)
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
          }
     }

     env::mtx.lock();
     env::move = POSINIT;
     env::flag = 1;
     env::mtx.unlock();

     file_.close();
}

void
Client::restoreFile()
{
     std::string cmd;

     if (file_.is_open())
          file_.close();
     
     cmd = "cp -f " + config::dir + filename_ + " " + config::tmpdir + filename_;
     std::system(cmd.data());

     cmd = config::tmpdir + filename_;
     
     file_.open(cmd);

     if (!file_.fail()) {
          do {
               file_.clear();
               file_ >> env::move;
          } while (!file_.eof());
          env::move += '\n';
     } else
          env::posinit();

     env::flag = 1;
}

void
Client::saveFile()
{
     if (!file_.is_open())
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
     else
          list += '\n';

     std::closedir(dir);
     
     send(list.data(), list.size());
}

void
Client::setFilename(std::string filename)
{
     if (filename == "")
          filename_.clear();
     else {
          filename += ".h2kc";
          filename_ = filename;
     }
}

void
Client::setID(std::string id)
{
     id_ = id;
}

void
Client::setMode(int8_t mode)
{
     env::run = 0;

     mode_ = mode;
}

void
Client::wait()
{
     std::string wait = WAIT;

     if (file_.is_open()) {
          file_.flush();

          axes.b = env::posAxes.b;
          axes.e = env::posAxes.e;
          axes.c = env::posAxes.c;
          axes.t = env::posAxes.t;
          axes.r = env::posAxes.r;
          axes.p = env::posAxes.p;
     }
     
     send(wait.data(), wait.size());
}

void
Client::writeFile(std::string data)
{
     if (!file_.is_open())
          throw ClientException("writeFile", ENOENT);

     file_ << data;
}

void
Client::run(Client *cli, std::string cmd)
{
     uint64_t i;
     uint64_t nb;
     size_t pos;

     pos = cmd.find(';');
     nb = std::atoll(cmd.substr(0, pos).data());

     cli->setFilename(cmd.substr(pos+1, std::string::npos));

     try {
          for (i = 0; i < nb; i++) {
               env::mtx.lock();
               if (env::run < 1) {
                    env::mtx.unlock();
                    break;
               }
               env::mtx.unlock();

               cli->readFile(cmd.substr(pos+1, std::string::npos));
          }
     } catch (const ClientException& e) {
          std::cerr << e.what() << std::endl; 
     }

     cli->setFilename("");

     env::mtx.lock();
     env::run = 0;
     env::posinit();
     env::flag = 1;
     env::mtx.unlock();
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

               std::memset(buf, 0, sizeof buf);
               cli.recv(buf, sizeof buf - 1);

               env::mtx.lock();
               try {
                    TcpParser parser(cli, buf);

                    parser.check();
                    parser.parse();
               } catch (const ParserException& e) {
                    std::cerr << e.what() << std::endl;
               }
               env::mtx.unlock();
          }
     } catch (const SocketException& e) {
          std::cerr << e.what() << std::endl;
     }

     env::mtx.lock();
     if (&cli == env::client) {
          env::client = env::cliQueue.front();

          if (env::client != NULL) {
               env::cliQueue.pop_front();
               env::client->master();
          }
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

     *alive = false;
}
