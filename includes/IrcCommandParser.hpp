#ifndef IRCCOMMANDPARSER_HPP
#define IRCCOMMANDPARSER_HPP

#include "CommandFactory.hpp"
#include <string>

class IrcCommandParser {
private:
  std::string buffer;
  std::string command;
  std::string payload;

public:
  IrcCommandParser(std::string buffer);
  ~IrcCommandParser();
  CommandType getMessageType();
  std::string getPayload() { return payload; };
};

#endif