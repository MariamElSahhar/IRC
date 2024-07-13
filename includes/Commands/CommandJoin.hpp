#ifndef COMMANDJOIN_HPP
#define COMMANDJOIN_HPP

#include "ICommand.hpp"

class CommandJoin : public ICommand {
 public:
  CommandJoin();
  virtual ~CommandJoin();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::string payload);
};

#endif
