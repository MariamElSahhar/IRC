#include "CommandQuit.hpp"
#include "Channel.hpp"
#include "Server.hpp"

CommandQuit::CommandQuit() {}
CommandQuit::~CommandQuit() {}

void CommandQuit::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *param) {
  (void)param;
  (void)clientSocket;
  std::string quit = "QUIT :leaving\r\n";

  std::vector<Channel *> channels = server->list_channels();
  for (std::vector<Channel *>::iterator it = channels.begin();
       it != channels.end(); it++) {
    Channel *channel = *it;
    if (channel->has_client(client)) {
      channel->quit(client);
    }
  }
  server->delete_client_by_nickname(client->get_nickname(), quit);
}
