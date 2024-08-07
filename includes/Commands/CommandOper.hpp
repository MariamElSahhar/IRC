#ifndef COMMANDOPER_HPP
#define COMMANDOPER_HPP

#include "ICommand.hpp"

class CommandOper : public ICommand {
 public:
  CommandOper();
  virtual ~CommandOper();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);
};

#endif
