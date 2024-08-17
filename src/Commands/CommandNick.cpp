#include "CommandNick.hpp"
#include "Server.hpp"

CommandNick::CommandNick() {}
CommandNick::~CommandNick() {}

bool CommandNick::validate_nick(std::string nick) {
  std::cout << nick << std::endl;
  if (nick.length() < 2 || nick.length() > 15 || !isalpha(nick[0]) ||
      nick.find("--") != std::string::npos ||
      nick.find("__") != std::string::npos) {
    return (false);
  }
  for (size_t i = 0; i < nick.length(); ++i) {
    if (!isalnum(nick[i]) && nick[i] != '-' && nick[i] != '_') {
      return (false);
    }
  }
  return (true);
}

void CommandNick::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (params->size() < 1) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("NICK", server->get_hostname()));
    return;
  }

  std::string nick = params->at(0);
  if (!validate_nick(nick)) {
    server->sendResponse(clientSocket,
                         ERR_ERRONEUSNICKNAME(server->get_hostname(), nick));
    return;
  }
  if (server->get_client_by_nickname(nick)) {
    server->sendResponse(clientSocket,
                         ERR_NICKNAMEINUSE(server->get_hostname(), nick));
    return;
  }
  client->set_nickname(nick);

  std::string response = "Nickname set to " + nick + "\r\n";
  server->sendResponse(clientSocket, response);
}
