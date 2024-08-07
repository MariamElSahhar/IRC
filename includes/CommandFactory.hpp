#ifndef COMMANDFACTORY_HPP
#define COMMANDFACTORY_HPP

#include "CommandInvite.hpp"
#include "CommandJoin.hpp"
#include "CommandKick.hpp"
#include "CommandNick.hpp"
#include "CommandMode.hpp"
#include "CommandPart.hpp"
#include "CommandPass.hpp"
#include "CommandPing.hpp"
#include "CommandNick.hpp"
#include "CommandUser.hpp"
#include "CommandPrivMsg.hpp"
#include "CommandTopic.hpp"
#include "CommandCap.hpp"
#include "CommandQuit.hpp"
#include "CommandOper.hpp"
#include "CommandUnoper.hpp"
#include "ICommand.hpp"

class ICommand;

enum CommandType {
  INVALID,
  INVITE,
  JOIN,
  KICK,
  MODE,
  PART,
  PASS,
  PRIVMSG,
  TOPIC,
  NICK,
  USER,
	OPER,
	UNOPER,
	CAP,
  PING,
  QUIT,
};

class CommandFactory {
 public:
  CommandFactory();
  ~CommandFactory();
  ICommand *createCommand(CommandType cmdType);
};

#endif
