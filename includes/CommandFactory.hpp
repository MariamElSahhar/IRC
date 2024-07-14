#ifndef COMMANDFACTORY_HPP
#define COMMANDFACTORY_HPP

#include "CommandInvite.hpp"
#include "CommandJoin.hpp"
#include "CommandKick.hpp"
#include "CommandMode.hpp"
#include "CommandPart.hpp"
#include "CommandPass.hpp"
#include "CommandNick.hpp"
#include "CommandPrivMsg.hpp"
#include "CommandTopic.hpp"
#include "ICommand.hpp"

class ICommand;

enum CommandType {
  INVALID,
  INVITE,
  JOIN,
  KICK,
  MODE,
  PART,  // check if needed
  PASS,
  PRIVMSG,
  TOPIC,
  NICK
};

class CommandFactory {
 public:
  CommandFactory();
  ~CommandFactory();
  ICommand *createCommand(CommandType cmdType);
};

#endif
