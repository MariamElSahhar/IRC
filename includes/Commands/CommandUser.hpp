#ifndef COMMANDUSER_HPP
#define COMMANDUSER_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class CommandUser : public ICommand {
 public:
  CommandUser();
  virtual ~CommandUser();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);

 private:
};

#endif
