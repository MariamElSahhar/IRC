#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"
#include "CommandFactory.hpp"
#include "IrcClients.hpp"
#include <arpa/inet.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

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

class Channel;

class Server {
 public:
  Server(int port,
         std::string password,
         IrcClients *ircClients,
         CommandFactory *commandFactory);
  ~Server();

  void start(); // main function of the server (will create the sockets);
  void createSocket(int fd);
  void waitConnections();
  void acceptConnection();
  std::string getPassword();
  std::string getHostname();
  int readMessage(int indexFd);


  Channel *get_channel(std::string name);
  void add_channel(Channel *channel);

  void sendResponse(int clientSocket, std::string msg);
  void cleanUp();
  Client *getClientByNickname(const std::string &nickname);
  void registerClient(int &clientSocket, Client *client);

 private:
  int portNb;
  int socketNb;
  std::string password;
  std::string ipAddress;
  std::string hostName;
  std::vector<pollfd> pollFdVector;
  IrcClients *ircClients;
  CommandFactory *commandFactory;
  std::vector<Channel *> _channels;

  Server();
  Server(Server const &);
  Server &operator=(Server const &);
};

#endif
