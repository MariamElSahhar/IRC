#include "CommandKick.hpp"
#include <vector>

CommandKick::CommandKick() {}
CommandKick::~CommandKick() {}
void CommandKick::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {}
