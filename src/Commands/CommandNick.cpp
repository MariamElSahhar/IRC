#include "CommandNick.hpp"
#include <vector>

CommandNick::CommandNick() {}
CommandNick::~CommandNick() {}

bool CommandNick::validate_nick(std::string nick) {
  if (nick.empty())
    return (false);
  return (true);
}

void CommandNick::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (!client->getAuthentication()) {
    server->sendResponse(clientSocket,
                         ERR_NOTREGISTERED(server->getHostname()));
  }
  if (params->size() < 1) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("NICK", server->getHostname()));
    return;
  }

  std::string newNick = params->at(0);
  if (!validate_nick(newNick)) {
    server->sendResponse(clientSocket,
                         ERR_ERRONEUSNICKNAME(server->getHostname(), newNick));
    return;
  }
  if (server->findNick(newNick)) {
    server->sendResponse(clientSocket,
                         ERR_ERRONEUSNICKNAME(server->getHostname(), newNick));
    return;
  }
  client->setNickname(newNick);

  std::string response = "Nickname set to " + newNick + "\r\n";
  server->sendResponse(clientSocket, response);
}
