#include "CommandCap.hpp"
#include "Server.hpp"

CommandCap::CommandCap() {}
CommandCap::~CommandCap() {}

void CommandCap::execute(int &clientSocket,
                         Client * /*client*/,
                         Server *server,
                         std::vector<std::string> * /*params*/) {
	// ERROR CODE IS PLACEHOLDER
	server->sendResponse(clientSocket, ":302:No supported capabilities available\r\n");
}
