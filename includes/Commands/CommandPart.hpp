#ifndef COMMANDPART_HPP
#define COMMANDPART_HPP

#include "ICommand.hpp"

class CommandPart : public ICommand {
 public:
  CommandPart();
  virtual ~CommandPart();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);
};

#endif
