#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "Client.hpp"

class Server;

class ICommand {
 public:
  virtual void execute(int &clientSocket,
                       Client *client,
                       Server *server,
                       std::string payload) = 0;
};

#endif
