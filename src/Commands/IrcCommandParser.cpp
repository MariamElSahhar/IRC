#include "IrcCommandParser.hpp"

void IrcCommandParser::tokenize(std::string buffer) {
  std::vector<std::string> split_buffer = split(buffer);

  if (split_buffer.size() < 1)
    throw std::runtime_error("Empty Message.");

  // Checking if the first token has a ':' or '/' and taking it out of vector
  if (split_buffer[0][0] == ':' || split_buffer[0][0] == '/') {
    _command = split_buffer[0].substr(1);
    split_buffer.erase(split_buffer.begin());
  }

  // Command should be stored in UPPER case format
  _command = split_buffer[0];
  std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);

  // Rest of the tokens are the payload
  for (size_t i = 1; i < split_buffer.size(); i++) {
    // If the token starts with ':' everything after is a single parameter
    if (split_buffer[i][0] == ':') {
      std::string param = split_buffer[i].substr(1);
      for (size_t k = i + 1; k < split_buffer.size(); k++)
        param += " " + split_buffer[k];
      _params.push_back(param);
      break;
    }
    _params.push_back(split_buffer[i]);
  }
  return;
}

IrcCommandParser::IrcCommandParser(std::string rawBuffer) : _buffer(rawBuffer) {
  try {
    tokenize(_buffer);
    std::cout << "Command identified: " << _command << std::endl;
  } catch (std::exception &e) {
    throw std::runtime_error("Parsing failed: " + std::string(e.what()));
  }
}

IrcCommandParser::~IrcCommandParser() {}

CommandType IrcCommandParser::getMessageType() {
  if (_command.compare("INVITE") == 0)
    return INVITE;
  if (_command.compare("KICK") == 0)
    return KICK;
  if (_command.compare("MODE") == 0)
    return MODE;
  if (_command.compare("TOPIC") == 0)
    return TOPIC;
  if (_command.compare("JOIN") == 0)
    return JOIN;
  if (_command.compare("PART") == 0)
    return PART;
  if (_command.compare("PRIVMSG") == 0)
    return PRIVMSG;
  if (_command.compare("PASS") == 0)
    return PASS;
  if (_command.compare("NICK") == 0)
    return NICK;
  return INVALID;
}
