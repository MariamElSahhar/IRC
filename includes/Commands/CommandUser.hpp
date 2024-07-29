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
  std::string parse_realname(std::vector<std::string> &params);
  bool validate_username(std::string username);
  bool allow_username(int &clientSocket,
                      Client *client,
                      Server *server,
                      std::vector<std::string> *params);
};

#endif
