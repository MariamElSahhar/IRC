#ifndef COMMANDNICK_HPP
#define COMMANDNICK_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class CommandNick : public ICommand {
 public:
  CommandNick();
  virtual ~CommandNick();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);

 private:
  bool validate_nick(std::string nick);
};

#endif
