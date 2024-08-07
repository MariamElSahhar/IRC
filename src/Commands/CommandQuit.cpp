#include "CommandQuit.hpp"
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

  server->delete_client_by_nickname(client->get_nickname(), quit);
}
