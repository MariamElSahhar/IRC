#ifndef COMMANDPRIVMSG_HPP
#define COMMANDPRIVMSG_HPP

#include "Channel.hpp"
#include "ICommand.hpp"
#include "Server.hpp"

class CommandPrivMsg : public ICommand {
 public:
  CommandPrivMsg();
  virtual ~CommandPrivMsg();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::vector<std::string> *params);

 private:
  bool validate_command(int &clientSocket,
                        Client *client,
                        Server *server,
                        std::vector<std::string> &params);
  void send_to_channel(int &clientSocket,
                       Client *client,
                       Server *server,
                       std::string &recipeint,
                       std::string &message);
  void send_to_user(int &clientSocket,
                    Client *client,
                    Server *server,
                    std::string &recipeint,
                    std::string &message);
  std::string &parse_message(std::vector<std::string> &params);
};

#endif
