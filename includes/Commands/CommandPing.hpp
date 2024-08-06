#ifndef COMMANDPING_HPP
#define COMMANDPING_HPP

#include "ICommand.hpp"

class CommandPing: public ICommand {
 public:
  CommandPing();
  virtual ~CommandPing();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);
};

#endif