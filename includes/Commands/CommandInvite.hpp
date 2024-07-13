#ifndef COMMANDINVITE_HPP
#define COMMANDINVITE_HPP

#include "ICommand.hpp"

class CommandInvite : public ICommand {
 public:
  CommandInvite();
  virtual ~CommandInvite();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::string payload);
};

#endif
