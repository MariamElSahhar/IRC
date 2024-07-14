#include "CommandPrivMsg.hpp"
#include <vector>

CommandPrivMsg::CommandPrivMsg() {}
CommandPrivMsg::~CommandPrivMsg() {}
void CommandPrivMsg::execute(int &clientSocket,
                             Client *client,
                             Server *server,
                             std::vector<std::string> *params) {}
