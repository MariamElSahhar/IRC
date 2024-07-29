#include "IrcClients.hpp"

IrcClients::IrcClients() {}

void IrcClients::createClient(int clientSocket, std::string ipClient) {
  std::cout << "IrcClients::createClient" << std::endl;
  Client *client = new Client(clientSocket, ipClient);
  mapSocketToClientInstance[clientSocket] = client;
}

Client *IrcClients::getClient(int clientSocket) {
  if (mapSocketToClientInstance.find(clientSocket) !=
      mapSocketToClientInstance.end())
    return (mapSocketToClientInstance[clientSocket]);
  return (NULL);
}

bool IrcClients::removeClient(int clientSocket) {
  std::map<int, Client *>::iterator it =
      mapSocketToClientInstance.find(clientSocket);
  if (it != mapSocketToClientInstance.end()) {
    delete (it->second);
    mapSocketToClientInstance.erase(it);
    return true;
  }
  return false;
}

IrcClients::~IrcClients() {
  if (!mapSocketToClientInstance.empty()) {
    std::map<int, Client *>::iterator it;
    for (it = mapSocketToClientInstance.begin();
         it != mapSocketToClientInstance.end(); it++) {
      delete (it->second);
    }
    mapSocketToClientInstance.clear();
  }
}

Client *IrcClients::getClientByNickname(const std::string &nickname) {
  std::map<int, Client *>::iterator it;
  for (it = mapSocketToClientInstance.begin();
       it != mapSocketToClientInstance.end(); ++it) {
    if (it->second->get_nickname() == nickname) {
      return (it->second);
    }
  }
  return (NULL);
}
