#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "Client.hpp"

class Server;

class ICommand {
 public:
  virtual void execute(int &clientSocket,
                       Client *client,
                       Server *server,
                       std::vector<std::string> *param) = 0;
	virtual ~ICommand() {};
};

#endif
