#include "CommandPrivMsg.hpp"

CommandPrivMsg::CommandPrivMsg() {}
CommandPrivMsg::~CommandPrivMsg() {}

bool CommandPrivMsg::validate_command(int &clientSocket,
                                      Client *client,
                                      Server *server,
                                      std::vector<std::string> &params) {
  if (!client->is_registered()) {
    server->sendResponse(clientSocket,
                         ERR_NOTREGISTERED(server->get_hostname()));
    return (false);
  }
  if (params.size() < 2 || params[0].empty() || params[1].empty()) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("NICK", server->get_hostname()));
    return (false);
  }
  if (params[0].length() < 2) {
    server->sendResponse(clientSocket,
                         ERR_NORECIPIENT("NICK", server->get_hostname()));
  }
  if (params.size() < 3 && params[1].length() < 2) {
    server->sendResponse(clientSocket,
                         ERR_NOTEXTTOSEND(server->get_hostname()));
  }
  return (true);
}

std::string &CommandPrivMsg::parse_message(std::vector<std::string> &params) {
  std::string message;
  for (size_t i = 1; i < params.size(); ++i) {
    if (i > 1) {
      message += " ";
    }
    message += params[i];
  }

  if (!message.empty() && message[0] == ':') {
    message = message.substr(1);
  }
  if (!message.empty() && message[message.size() - 1] == ':') {
    message = message.substr(0, message.size() - 1);
  }
  return message;
}

void CommandPrivMsg::send_to_channel(int &clientSocket,
                                     Client *client,
                                     Server *server,
                                     std::string &recipient,
                                     std::string &message) {
  Channel *channel = server->get_channel(recipient);
  if (!channel) {
    server->sendResponse(clientSocket,
                         ERR_NOSUCHCHANNEL(recipient, server->get_hostname()));
    return;
  }
  if (!channel->has_client(client)) {
    server->sendResponse(clientSocket, ERR_CANNOTSENDTOCHAN(recipient));
    return;
  }
  channel->broadcast(client, "PRIVMSG", "", message);
}

void CommandPrivMsg::send_to_user(int &clientSocket,
                                  Client *client,
                                  Server *server,
                                  std::string &recipient_nick,
                                  std::string &message) {
  Client *recipient = server->get_client_by_nickname(recipient_nick);
  if (!recipient) {
    server->sendResponse(
        clientSocket, ERR_NOSUCHNICK(recipient_nick, server->get_hostname()));
    return;
  }
  server->sendResponse(recipient->get_socket(), PRIVMSG(client->get_nickname(),
                                             recipient_nick, message));
}

void CommandPrivMsg::execute(int &clientSocket,
                             Client *client,
                             Server *server,
                             std::vector<std::string> *params) {
  if (!validate_command(clientSocket, client, server, *params))
    return;
  std::string message = parse_message(*params);
  if (params->at(0)[0] == '#') {
    send_to_channel(clientSocket, client, server, params->at(0), message);
  } else {
    send_to_user(clientSocket, client, server, params->at(0), message);
  }
}
