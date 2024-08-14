#include "CommandUser.hpp"
#include "Server.hpp"

CommandUser::CommandUser() {}
CommandUser::~CommandUser() {}

std::string CommandUser::parse_realname(std::vector<std::string> &params) {
  std::string realname;
  for (size_t i = 3; i < params.size(); ++i) {
    if (i > 3) {
      realname += " ";
    }
    realname += params[i];
  }

  if (!realname.empty() && realname[0] == ':') {
    realname = realname.substr(1);
  }
  if (!realname.empty() && realname[realname.size() - 1] == ':') {
    realname = realname.substr(0, realname.size() - 1);
  }
  return realname;
}

// Validation needs cleanup
bool CommandUser::validate_command(int &clientSocket,
                                   Client *client,
                                   Server *server,
                                   std::vector<std::string> &params) {
  // if NICK isn't set
  if (client->get_nickname().empty()) {
    server->sendResponse(clientSocket,
                         ERR_NONICKNAMEGIVEN(server->get_hostname()));
    return (false);
  }
  // wrong number of parameteres
  if (params.size() < 4) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("USER", server->get_hostname()));
    return (false);
  }
  return (true);
}

void CommandUser::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  // if USER already set
  if (client->is_registered())
    server->sendResponse(clientSocket,
                         ERR_ALREADYREGISTERED(server->get_hostname()));
  if (!validate_command(clientSocket, client, server, *params)) {
    server->delete_client_by_nickname(
        client->get_nickname(),
        "Server disconnected due to authentication failure\r\n");
    return;
  }

  // assigning parameters to variables
  std::string username = params->at(0);
  std::string hostname = params->at(1);
  std::string servername = params->at(2);
  std::string realname = parse_realname(*params);

  // validating parameters
  if (username.empty() || hostname.empty() || servername.empty() ||
      realname.empty()) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("USER", server->get_hostname()));
    server->delete_client_by_nickname(
        client->get_nickname(),
        "Server disconnected due to authentication failure\r\n");
    return;
  }

  // assigning parameters to client attributes
  if (client->is_authenticated() == true) {
    client->set_username(username);
    client->set_realname(realname);
    client->set_hostname(hostname);
    client->set_servername(servername);
    client->register_client();
  } else
    server->delete_client_by_nickname(
        client->get_nickname(),
        "Server disconnected due to authentication failure\r\n");
}
