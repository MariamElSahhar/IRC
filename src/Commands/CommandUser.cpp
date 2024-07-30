#include "CommandUser.hpp"

CommandUser::CommandUser() {}
CommandUser::~CommandUser() {}

std::string CommandUser::parse_realname(std::vector<std::string> &params) {
  std::string realname = "";
  for (size_t i = 3; i < params.size(); ++i) {
    if (i > 3) {
      realname += " ";
    }
    realname += params[i];
  }

  if (!realname.empty() && realname.front() == ':') {
    realname.erase(realname.begin());
  }
  if (!realname.empty() && realname.back() == ':') {
    realname.pop_back();
  }

  return realname;
}

bool CommandUser::allow_username(int &clientSocket,
                                 Client *client,
                                 Server *server,
                                 std::vector<std::string> *params) {
  // if USER already set
  if (client->getRegistration()) {
    server->sendResponse(clientSocket,
                         ERR_ALREADYREGISTERED(server->getHostname()));
    return (false);
  }
  // if PASS isn't set
  if (!client->getAuthentication()) {
    server->sendResponse(clientSocket,
                         ERR_NOTREGISTERED(server->getHostname()));
    return (false);
  }
  // if NICK isn't set
  if (client->getNickname().empty()) {
    server->sendResponse(clientSocket,
                         ERR_NONICKNAMEGIVEN(server->getHostname()));
    return (false);
  }
  // wrong number of parameteres
  if (params->size() < 4) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("USER", server->getHostname()));
    return (false);
  }
  return (true);
}

void CommandUser::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (!allow_username(clientSocket, client, server, params))
    return;

  // assigning parameters to variables
  std::string username = params->at(0);
  std::string hostname = params->at(1);
  std::string servername = params->at(2);
  std::string realname = parse_realname(*params);

  // validating parameters
  if (username.empty() || hostname.empty() || servername.empty() ||
      realname.empty()) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("USER", server->getHostname()));
    return;
  }

  // assigning parameters to client attributes
  client->setUsername(username);
  client->setRealname(realname);
  client->setHostname(hostname);
  client->setServername(servername);
  server->registerClient(clientSocket, client);
}
