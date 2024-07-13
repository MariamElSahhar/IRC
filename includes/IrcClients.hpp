#ifndef IRC_CLIENTS_HPP
#define IRC_CLIENTS_HPP

#include "Client.hpp"
#include <map>
#include <string>

class IrcClients {
public:
  IrcClients();
  ~IrcClients();
  void createClient(int clientSocket, std::string ipClient);
  Client *getClient(int clientSocket);
  bool removeClient(int clientSocket);

private:
  std::map<int, Client *> mapSocketToClientInstance;
};

#endif