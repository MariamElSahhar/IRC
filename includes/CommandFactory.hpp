#ifndef COMMANDFACTORY_HPP
#define COMMANDFACTORY_HPP

#include "ICommand.hpp"

class ICommand;

enum CommandType {
  INVALID,
  INVITE,
  JOIN,
  KICK,
  MODE,
  PART, // check if needed
  PASS,
  PRIVMSG,
  TOPIC
};

class CommandFactory {
public:
  CommandFactory();
  ~CommandFactory();
  ICommand *createCommand(CommandType cmdType);
};

#endif
