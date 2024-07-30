#include "Client.hpp"
#include "Server.hpp"

Client::Client(int fd, std::string ip) {
  (void)fd;
  (void)ip;
  isAuthenticated = false;  // using in PASS Command (setting to false in the
                            // beginning and true after validation)
  // true after user using NICK and USER Commands
  isRegistered = false;
  // user attributes
  _nickname = "";
  _username = "";
  _hostname = "";
  _servername = "";
  _realname = "";
}

Client::~Client() {}

void Client::registerClient() {
  isRegistered = true;
}

void Client::authenticate() {
  isAuthenticated = true;
}

bool Client::getAuthentication() {
  return (isAuthenticated);
}

bool Client::getRegistration() {
  return (isRegistered);
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
      (*fullMessage.rbegin() == '\n' || *fullMessage.rbegin() == '\r'))
    fullMessage.erase(fullMessage.length() - 1);
  currentMessage.clear();  // Clear the current message
}

void Client::setNickname(std::string nickname) {
  _nickname = nickname;
}

std::string Client::getNickname() {
  return (_nickname);
}

void Client::setUsername(std::string username) {
  _username = username;
}

std::string Client::getUsername() {
  return (_username);
}

void Client::setRealname(std::string realname) {
  _realname = realname;
}

std::string Client::getRealname() {
  return (_realname);
}

void Client::setHostname(std::string hostname) {
  _hostname = hostname;
}

std::string Client::getHostname() {
  return (_hostname);
}

void Client::setServername(std::string servername) {
  _servername = servername;
}

std::string Client::getServername() {
  return (_servername);
}
