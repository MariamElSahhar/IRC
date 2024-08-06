#ifndef COMMANDQUIT_HPP
#define COMMANDQUIT_HPP

#include "ICommand.hpp"

class CommandQuit: public ICommand {
 public:
  CommandQuit();
  virtual ~CommandQuit();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);
};

#endif