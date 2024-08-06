#include "CommandPing.hpp"
#include "Server.hpp"

CommandPing::CommandPing() {}
CommandPing::~CommandPing() {}

void CommandPing::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *param) {

	std::string ping = "PONG " + param->at(0) + "\r\n";

	server->sendResponse(clientSocket, ping);
}