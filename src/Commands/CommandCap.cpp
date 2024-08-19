#include "CommandCap.hpp"
#include "Server.hpp"

CommandCap::CommandCap() {}
CommandCap::~CommandCap() {}

void CommandCap::execute(int &clientSocket,
                         Client * client,
                         Server *server,
                         std::vector<std::string> * params) {
  (void)params;
  (void)client;

	server->sendResponse(clientSocket, "CAP * LS :\r\n");
}
