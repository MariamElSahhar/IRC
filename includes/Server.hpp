#ifndef SERVER_HPP
#define SERVER_HPP

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
#include "utils.hpp"

#define MAX_CON 100
#define MAX_TOPIC_LEN 307
#define MAX_MESSAGE_SIZE 307
#define MAX_BUF 512
#define HOSTNAME "127.0.0.1"
#define IP_BIND "127.0.0.1"

// limits: https://www.geekshed.net/tag/limits/

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

  void start();  // main function of the server (will create the sockets);
  void createSocket(int fd);
  void waitConnections();
  void acceptConnection();
  std::string getPassword();
  std::string get_hostname();
  int readMessage(int indexFd);

  Channel *get_channel(std::string name);
  void add_channel(Channel *channel);

  void sendResponse(int clientSocket, std::string msg);
  void cleanUp();
  Client *get_client_by_nickname(const std::string &nickname);

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
