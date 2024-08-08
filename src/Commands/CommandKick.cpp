#include "CommandKick.hpp"
#include "Channel.hpp"
#include "Server.hpp"

CommandKick::CommandKick() {}
CommandKick::~CommandKick() {}
void CommandKick::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (client->is_authenticated() && client->is_registered()) {
    if (params->size() < 2) {
      server->sendResponse(clientSocket,
                           ERR_NEEDMOREPARAMS("KICK", server->get_hostname()));
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
    // Checking if client is channel operator
    if (!client->is_operator() &&
        !channel->is_channel_operator(client->get_nickname())) {
      server->sendResponse(clientSocket, ERR_NOPRIVILEGES(channel->get_name()));
      return;
    }
    // Checking if target client exists
    Client *target = server->get_client_by_nickname((*params)[1]);
    if (target == NULL) {
      server->sendResponse(
          clientSocket, ERR_NOSUCHNICK((*params)[1], server->get_hostname()));
      return;
    }
    // Check if target is in Channel
    if (!channel->has_client(target)) {
      server->sendResponse(
          clientSocket,
          ERR_USERNOTINCHANNEL(target->get_nickname(), channel->get_name(),
                               server->get_hostname()));
      return;
    }
    // Kick the target from the channel
    std::string reason;
    if (params->size() > 2)
      reason = (*params)[2];
    else
      reason = "Kicked";
    channel->kick(*server, target, client, reason);
  } else
    server->sendResponse(
        clientSocket,
        ERR_NOTREGISTERED(client->get_hostname(), client->get_nickname()));
}
