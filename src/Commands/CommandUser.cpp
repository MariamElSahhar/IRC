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
                         ERR_NEEDMOREPARAMS("NICK", server->getHostname()));
    return;
  }

  std::string nick = params->at(0);

  client->setUsername(nick);
  client->setRealname(nick);

  std::string response = "Nickname set to " + nick + "\r\n";
  server->sendResponse(clientSocket, response);
}
