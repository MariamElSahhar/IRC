#ifndef COMMANDCAP_HPP
#define COMMANDCAP_HPP

#include "ICommand.hpp"

class CommandCap: public ICommand {
 public:
  CommandCap();
  virtual ~CommandCap();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);
};

#endif
