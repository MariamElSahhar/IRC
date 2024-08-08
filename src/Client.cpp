#include "Client.hpp"
#include "IrcClients.hpp"
#include "Server.hpp"

Client::Client(int fd, Server &server, std::string ip) {
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
  _disconnected = false;
}

Client::~Client() {}

void Client::register_client() {
  _server->sendResponse(_socket,
                        RPL_WELCOME(get_nickname(), _server->get_hostname()));
  _server->sendResponse(_socket,
                        RPL_YOURHOST(_server->get_hostname(), get_nickname()));
  _server->sendResponse(_socket,
                        RPL_CREATED(_server->get_hostname(), get_nickname()));
  _server->sendResponse(_socket,
                        RPL_MYINFO(_server->get_hostname(), get_nickname()));
  _registered = true;
}

// DEPRACATED --> USE Server::delete_client_by_nickname()
void Client::disconnect(std::string reason) {
  if (_disconnected)
    return;
  _disconnected = true;
  _server->sendResponse(_socket, YELLOW + reason + RESET);
  close(_socket);
}

void Client::authenticate() {
  _authenticated = true;
}

bool Client::is_registered(void) const {
  return _registered;
}

bool Client::is_authenticated() const {
  return (_authenticated);
}

int Client::get_socket() const {
  return (_socket);
}

bool Client::is_operator(void) const {
  return _operator;
}

bool Client::is_disconnected(void) const {
  return _disconnected;
}

bool Client::isMessageReady() {
  return (this->currentMessage.empty());
}

void Client::set_buffer(std::string buffer) {
  this->currentMessage += buffer;
}

std::string Client::get_buffer(void) const {
  return (this->currentMessage);
}

void Client::clear_buffer(void) {
  this->currentMessage.clear();
}

// DEPRACATED Please use server.sendResponse()
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

std::string Client::get_servername() const {
  return (_servername);
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

void Client::set_operator(std::string oper_password) {
  if (oper_password == g_oper_password)
    _operator = true;
}

void Client::unset_operator() {
  _operator = false;
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

void Client::set_servername(std::string servername) {
  _servername = servername;
}

void Client::set_hostname(std::string hostname) {
  _hostname = hostname;
}

void Client::broadcast(Client *sender,
                       std::string command,
                       std::string target,
                       std::string message) {
  // Client->Client or Client->Channel broadcast
  std::string sender_str;
  std::string command_str;
  std::string target_str;
  std::string message_str;

  sender_str = ":" + sender->get_nickname() + "!" + sender->get_username() +
               "@" + sender->get_hostname() + " ";
  command_str = command + " ";
  target_str = target + " ";
  if (command == "KICK" || command == "INVITE" || message.empty() ||
      message[0] == ':')
    message_str = message;
  else
    message_str = ":" + message;

  // Format ":<sender> <command> <target> :<message>\r\n"
  std::string reply =
      sender_str + command_str + target_str + message_str + "\r\n";

  std::cout << "Broadcast: " << reply << std::endl;

  send(_socket, reply.c_str(), reply.length(), 0);
  return;
}

std::string Client::generatePrefix() {
  std::string prefix = _nickname + "!" + _username + "@" + _server_hostname;
  return (prefix);
}
