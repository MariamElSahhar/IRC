#ifndef COMMANDTOPIC_HPP
#define COMMANDTOPIC_HPP

#include "ICommand.hpp"

class CommandTopic : public ICommand {
 public:
  CommandTopic();
  virtual ~CommandTopic();
  void execute(int &clientSocket,
               Client *client,
               Server *server,
               std::string payload);
};

#endif
