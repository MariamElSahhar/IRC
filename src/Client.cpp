#include "Client.hpp"
#include "ErrorCodes.hpp"
#include "Server.hpp"

Client::Client(int fd,  Server &server, std::string ip) {
  	_server = &server;
  _socket = fd;
  _server_hostname = ip;
  (void)fd;
  (void)ip;
  _nickname = "";
  _username = "";
  _hostname = "";
  _servername = "";
  _realname = "";

  _operator = false;
	_registered = false;
  _authenticated = false;
}

Client::~Client() {}

void Client::registerClient() {
  _registered = true;
}

void Client::authenticate() {
  _authenticated = true;
}

void Client::register_client() {
	if (get_nickname().empty())
		_server->sendResponse(_socket, ERR_NONICKNAMEGIVEN(_server_hostname));
	else if (!this->get_username().empty() && !this->get_realname().empty()) {
		_server->sendResponse(_socket, RPL_WELCOME(_nickname, _server_hostname));
		_registered = true;
	}
}

bool Client::is_registered(void) const {
	return _registered;
}

bool Client::get_Authentication() const {
  return (_authenticated);
}

int Client::get_socket() const {
  return (_socket);
}

bool Client::is_operator(void) const {
  return _operator;
}


bool Client::isMessageReady() {
  return (!this->fullMessage.empty());
}

std::string Client::get_EntireMessage() {
  return (fullMessage);
}

std::string Client::get_nickname(void) const {
  return _nickname;
}

std::string Client::get_username(void) const {
  return _username;
}

std::string Client::get_hostname(void) const {
  return _server_hostname;
}

std::string Client::get_realname(void) const {
  return _realname;
}

void Client::set_nickname(std::string nickname) {
  _nickname = nickname;
}

void Client::set_username(std::string username) {
  _username = username;
}

void Client::set_realname(std::string realname) {
  _realname = realname;
}

void Client::set_operator(std::string oper_password) {
  if (oper_password == g_oper_password)
    _operator = true;
}

void Client::unset_operator() {
	_operator = false;
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

void Client::reply(std::string code, std::string msg) {
  std::string hostname_str = ":" + _server_hostname + " ";
  std::string code_str;
  std::string nickname_str;

  if (code.empty())
    code_str = "";
  else
    code_str = code + " ";

  if (_nickname.empty())
    nickname_str = "unregistered ";
  else
    nickname_str = _nickname + " ";

  std::string reply = hostname_str + code_str + nickname_str + msg + "\r\n";
  std::cout << "Reply: " << reply << std::endl;
  send(_socket, reply.c_str(), reply.length(), 0);
}

void Client::setNickname(std::string nickname)
{
  _nickname = nickname;
}

std::string Client::getNickname()
{
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
