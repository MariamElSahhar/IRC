#include "IrcCommandParser.hpp"
#include <iostream>
#include <string>

IrcCommandParser::IrcCommandParser(std::string rawBuffer) : buffer(rawBuffer) {
  std::cout << "IrcCommandParser Parsing message: " << rawBuffer << std::endl;
  if (buffer.length() > 0 && buffer[0] == '/')
    buffer.erase(0, 1);
  int i = 0;
  for (i = 0; i < (int)buffer.length(); i++) {
    if (isspace(buffer[i]) != 0 || buffer[i] == '\n')
      break;
  }
  command.append(buffer, 0, i);
  std::cout << "Command identified:" << command << std::endl;
  if (i != (int)buffer.length())
    this->payload.append(buffer.begin() + i + 1, buffer.end());
  else
    this->payload.clear();
}

IrcCommandParser::~IrcCommandParser() {}

CommandType IrcCommandParser::getMessageType() {
  if (command.compare("INVITE") == 0)
    return INVITE;
  if (command.compare("KICK") == 0)
    return KICK;
  if (command.compare("MODE") == 0)
    return MODE;
  if (command.compare("TOPIC") == 0)
    return TOPIC;
  if (command.compare("JOIN") == 0)
    return JOIN;
  if (command.compare("PART") == 0)
    return PART;
  if (command.compare("PRIVMSG") == 0)
    return PRIVMSG;
  if (command.compare("PASS") == 0)
    return PASS;
  return INVALID;
}
