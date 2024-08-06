#include "CommandFactory.hpp"

CommandFactory::CommandFactory() {}

CommandFactory::~CommandFactory() {}

ICommand *CommandFactory::createCommand(CommandType cmdType) {
  ICommand *command;
  switch (cmdType) {
    case INVITE:
      command = new CommandInvite();
      break;
    case JOIN:
      command = new CommandJoin();
      break;
    case KICK:
      command = new CommandKick();
      break;
    case MODE:
      command = new CommandMode();
      break;
    case TOPIC:
      command = new CommandTopic();
      break;
    case PART:
      command = new CommandPart();
      break;
    case PRIVMSG:
      command = new CommandPrivMsg();
      break;
    case PASS:
      command = new CommandPass();
      break;
    case NICK:
      command = new CommandNick();
      break;
    case USER:
      command = new CommandUser();
      break;
    case CAP:
      command = new CommandCap();
      break;
    case PING:
      command = new CommandPing();
    default:
    case QUIT:
      command = new CommandQuit();
      break;

  }
  return command;
}
