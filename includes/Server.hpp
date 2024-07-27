#ifndef SERVER_HPP
#define SERVER_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include "CommandFactory.hpp"
#include "IrcClients.hpp"

#define MAX_CON 100
#define MAX_BUF 512
#define HOSTNAME "127.0.0.1"
#define IP_BIND "127.0.0.1"

// Color Macros
#define RESET "\e[m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define CYAN "\e[36m"

class CommandFactory;

class Server {
 public:
  Server(int port,
         std::string password,
         IrcClients *ircClients,
         CommandFactory *commandFactory);
  ~Server();

  void start();  // do all to start server
  void createSocket(int fd);
  void waitConnections();
  void acceptConnection();
  std::string getPassword();
  std::string getHostname();
  void readMessage(int indexFd);
  void sendResponse(int clientSocket, std::string msg);
  void cleanUp();

 private:
  int portNb;
  int socketNb;
  std::string password;
  std::string ipAddress;
  std::string hostName;
  std::vector<pollfd> pollFdVector;
  IrcClients *ircClients;
  CommandFactory *commandFactory;

  Server();
  Server(Server const &);
  Server &operator=(Server const &);
};

#endif
