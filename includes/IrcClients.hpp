#ifndef IRC_CLIENTS_HPP
#define IRC_CLIENTS_HPP

#include <map>
#include "Client.hpp"

class Server;
class IrcClients {
 public:
  IrcClients();
  ~IrcClients();
  void createClient(int clientSocket, Server &server, std::string ipClient);
  Client *getClient(int clientSocket);
	Client *getClientByNickname(const std::string &nickname);
  bool removeClient(int clientSocket);

 private:
  std::map<int, Client *> mapSocketToClientInstance; // int -> fd number of each Client
};

#endif
