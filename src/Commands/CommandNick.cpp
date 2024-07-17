#include "CommandNick.hpp"
#include <vector>

CommandNick::CommandNick() {}
CommandNick::~CommandNick() {}

bool CommandNick::validate_nick(std::string nick) {
  if (nick.length() < 2 || nick.length() > 15 ||
      !isalpha(nick[0] || nick.find("--") != std::string::npos ||
               nick.find("__") != std::string::npos)) {
    return false;
  }
  for (size_t i = 0; i < nick.length(); ++i) {
    if (!isalnum(nick[i]) && nick[i] != '-' && nick[i] != '_') {
      return false;
    }
  }
  return (true);
}

void CommandNick::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (!client->getAuthentication()) {
    server->sendResponse(clientSocket,
                         ERR_NOTREGISTERED(server->getHostname()));
    return;
  }
  if (params->size() < 1) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("NICK", server->getHostname()));
    return;
  }

  std::string nick = params->at(0);
  if (!validate_nick(nick)) {
    server->sendResponse(clientSocket,
                         ERR_ERRONEUSNICKNAME(server->getHostname(), nick));
    return;
  }
  if (server->getClientByNickname(nick)) {
    server->sendResponse(clientSocket,
                         ERR_NICKNAMEINUSE(server->getHostname(), nick));
    return;
  }
  client->setNickname(nick);

  std::string response = "Nickname set to " + nick + "\r\n";
  server->sendResponse(clientSocket, response);
}