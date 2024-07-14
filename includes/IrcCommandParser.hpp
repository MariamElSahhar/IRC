#ifndef IRCCOMMANDPARSER_HPP
#define IRCCOMMANDPARSER_HPP

#include "CommandFactory.hpp"

class IrcCommandParser {
 private:
  std::string _buffer;
  std::string _command;
  std::vector<std::string> _params;

  void tokenize(std::string buffer);

 public:
  IrcCommandParser(std::string buffer);
  ~IrcCommandParser();
  CommandType getMessageType();
  std::vector<std::string> getParams() { return _params; };
};

#endif
