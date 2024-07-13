#include "Client.hpp"
#include "Server.hpp"

Client::Client(int fd, std::string ip) {
    (void) fd;
    (void) ip;
    isAuthenticated = false; // using in PASS Command (setting to false in the begging and true after validation)
}

Client::~Client() {}

void Client::authenticate() {
    isAuthenticated = true;
}

bool Client::getAuthentication() {
    return (isAuthenticated);
  }

bool Client::isMessageReady() {
  return (!this->fullMessage.empty());
}

std::string Client::getEntireMessage() {
  return (fullMessage);
}

void Client::messageHandler(char msg[]) {
  if (!fullMessage.empty()) {
    fullMessage.clear();
  }

  if (strstr(msg, "\n") == 0) {
    currentMessage.append(msg);
  } else {
    processMessage(msg);
  }
  memset(msg, 0, MAX_BUF);
}

void Client::processMessage(const char *msg) {
  currentMessage.append(
      msg);  // Append the incoming message to the current message
  fullMessage.append(
      currentMessage);  // Append the current message to the full message

  if (!fullMessage.empty() &&
      (fullMessage.back() == '\n' ||
       fullMessage.back() ==
           '\r'))  // Remove trailing newline or carriage return characters
  {
    fullMessage.pop_back();
  }
  currentMessage.clear();  // Clear the current message
}
