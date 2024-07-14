#include "CommandTopic.hpp"
#include <vector>

CommandTopic::CommandTopic() {}
CommandTopic::~CommandTopic() {}
void CommandTopic::execute(int &clientSocket,
                           Client *client,
                           Server *server,
                           std::vector<std::string> *params) {}
