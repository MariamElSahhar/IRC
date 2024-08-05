#include "CommandMode.hpp"
#include "Channel.hpp"
#include "Server.hpp"

CommandMode::CommandMode() {}
CommandMode::~CommandMode() {}
void CommandMode::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (client->is_authenticated() == true && client->is_registered() == true) {
    if (params->size() < 1) {
      server->sendResponse(clientSocket,
                           ERR_NEEDMOREPARAMS("MODE", server->get_hostname()));
      return;
    }
    std::string channel_name = (*params)[0];
    Channel *channel = server->get_channel(channel_name);
    // Checking if Channel exists
    if (channel == NULL) {
      server->sendResponse(
          clientSocket,
          ERR_NOSUCHCHANNEL(channel_name, server->get_hostname()) +
              channel_name + "\r\n");
      return;
    }
    // Checking if client is in the channel
    if (!channel->has_client(client)) {
      server->sendResponse(
          clientSocket,
          ERR_NOTONCHANNEL(channel->get_name(), server->get_hostname()));
      return;
    }
    // Checking if command is issued for listing channel modes
    if (params->size() == 1) {
      server->sendResponse(
          clientSocket, RPL_MODE(channel->get_modes(), client->get_nickname(),
                                 channel->get_name()));
      return;
    }
    // Checking if client is channel operator
    if (!client->is_operator() &&
        !channel->is_channel_operator(client->get_nickname())) {
      server->sendResponse(clientSocket, ERR_NOPRIVILEGES(channel->get_name()));
      return;
    }
    std::string mode_flag = (*params)[1];
    if (mode_flag.length() != 2) {
      server->sendResponse(clientSocket, ERR_UNKNOWNMODE(channel->get_name()));
    }
    char mode = mode_flag[0];
    char flag = mode_flag[1];
    // Set or unset the mode
    if (mode == '-')
      channel->unset_mode(flag, *server, *params, client, channel->get_name());
    else if (mode == '+')
      channel->set_mode(flag, *server, *params, client, channel->get_name());
    else
      server->sendResponse(clientSocket, ERR_UNKNOWNMODE(channel->get_name()));
  } else {
    server->sendResponse(clientSocket,
                         ERR_NOTREGISTERED(server->get_hostname()));
  }
  return;
}
