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

/*
@MIkamal88 check this https://ircv3.net/specs/extensions/capability-negotiation.html
I undestand that we need to display CAP * LS : instead of the message.

means that we have no available capabilities
*/
