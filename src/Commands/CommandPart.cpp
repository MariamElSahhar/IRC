#include "CommandPart.hpp"
#include "Channel.hpp"
#include "Server.hpp"

CommandPart::CommandPart() {}
CommandPart::~CommandPart() {}
void CommandPart::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (client->is_authenticated() && client->is_registered()) {
    if (params->size() < 1) {
      server->sendResponse(clientSocket,
                           ERR_NEEDMOREPARAMS("PART", server->get_hostname()));
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
    if (params->size() > 1) {
      channel->part(client, (*params)[1]);
    } else {
      channel->part(client, "");
    }
  } else {
    server->sendResponse(
        clientSocket,
        ERR_NOTREGISTERED(client->get_hostname(), client->get_nickname()));
  }
}
