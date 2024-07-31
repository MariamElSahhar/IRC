#include "CommandPart.hpp"

CommandPart::CommandPart() {}
CommandPart::~CommandPart() {}
void CommandPart::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {}
