#include "CommandUser.hpp"

void CommandUser::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (!client->getAuthentication()) {
    server->sendResponse(clientSocket,
                         ERR_NOTREGISTERED(server->getHostname()));
    return;
  }
  if (params->size() < 4) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("USER", server->getHostname()));
    return;
  }

  std::string username = params->at(0);
  std::string realname = params->at(4);

  client->setUsername(username);
  client->setRealname(realname);

  std::string response = "Username set to " + username + "\r\n";
  server->sendResponse(clientSocket, response);
}
