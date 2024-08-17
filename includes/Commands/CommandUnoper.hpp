#ifndef COMMANDUNOPER_HPP
#define COMMANDUNOPER_HPP

#include "ICommand.hpp"

class CommandUnOper : public ICommand {
 public:
  CommandUnOper();
  virtual ~CommandUnOper();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);
};

#endif
