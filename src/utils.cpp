#include "utils.hpp"

std::vector<std::string> split(std::string buffer) {
  std::vector<std::string> tokens;
  std::string currentToken;
  bool inToken = false;

  for (size_t i = 0; i < buffer.size(); i++) {
    if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n' &&
        buffer[i] != '\r') {
      // Character is not whitespace, add it to the current token
      currentToken += buffer[i];
      inToken = true;
    } else if (inToken) {
      // We are transitioning from a non-whitespace character to whitespace
      // Push back the current token and reset it
      tokens.push_back(currentToken);
      currentToken.clear();
      inToken = false;
    }
  }
  // If the last character was part of a token, add it to the result
  if (!currentToken.empty())
    tokens.push_back(currentToken);
  return tokens;
}
