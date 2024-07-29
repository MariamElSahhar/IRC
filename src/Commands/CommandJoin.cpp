#include "CommandJoin.hpp"
#include "ErrorCodes.hpp"
#include "Server.hpp"

CommandJoin::CommandJoin() {}
CommandJoin::~CommandJoin() {}
void CommandJoin::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  // Check if client is authenticated AND **registered** ->> to be implemented
	client->authenticate(); // Temporary implementation until registration is implemented
  if (client->get_Authentication() == true) {
    if (params->size() < 1) {
      server->sendResponse(clientSocket,
                           ERR_NEEDMOREPARAMS("JOIN", server->getHostname()));
      return;
    }
    std::string channel_name = (*params)[0];
    if (channel_name[0] != '#') {
      server->sendResponse(clientSocket,
                           ERR_NOSUCHCHANNEL("JOIN", server->getHostname()));
      return;
    }
    Channel *channel = server->get_channel(channel_name);
    // Create channel if it does not exist, The first user to join creates it.
    if (channel == NULL) {
      if (!client->is_operator()) {
        server->sendResponse(clientSocket, ERR_NOPRIVILEGES(channel_name));
        return;
      }
      server->sendResponse(
          clientSocket, ERR_NOSUCHCHANNEL(channel_name, server->getHostname()));
      channel = new Channel(channel_name, server->getHostname());
      server->add_channel(channel);
      channel->increase_user_quantity();
      channel->join(client);
      channel->add_channel_operator(client);
    }
    // Join an already existent channel
    else {
      if (channel->has_client(client)) {
        server->sendResponse(clientSocket,
                             ERR_USERONCHANNEL("JOIN", channel->get_name(),
                                               server->getHostname()));
        return;
      }
      // Check if the client has the necessary permissions to join
      if (channel->get_invite_only() &&
          channel->get_invited_names().find(client->get_nickname()) ==
              std::string::npos) {
        server->sendResponse(clientSocket,
                             ERR_INVITEONLYCHAN(channel->get_name()));
        return;
      }
      if (channel->has_key() && (*params)[1] != channel->get_key()) {
        server->sendResponse(clientSocket,
                             ERR_BADCHANNELKEY(channel->get_name()));
        return;
      }
      if (channel->has_user_limit() &&
          channel->get_user_quantity() >= channel->get_user_limit()) {
        server->sendResponse(clientSocket,
                             ERR_CHANNELISFULL(channel->get_name()));
        return;
      }
      channel->increase_user_quantity();
      channel->join(client);
    }
    // Channel Topic and Names
    std::vector<std::string> param;
    param.push_back(channel->get_name());
    channel->topic(client, param);
    channel->names(client);
  } else
    server->sendResponse(clientSocket,
                         ERR_NOTREGISTERED(server->getHostname()));
}
