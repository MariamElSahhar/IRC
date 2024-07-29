#ifndef COMMANDPASS_HPP
#define COMMANDPASS_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class CommandPass : public ICommand {
 public:
  CommandPass();
  virtual ~CommandPass();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *param);
};

#endif
