#ifndef COMMANDMODE_HPP
#define COMMANDMODE_HPP

#include "ICommand.hpp"

class CommandMode : public ICommand {
 public:
  CommandMode();
  virtual ~CommandMode();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::string payload);
};

#endif
