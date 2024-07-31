#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(std::string name, std::string hostname, Server &server)
    : _server(&server),
      _name(name),
      _hostname(hostname),
      _topic(""),
      _pass(""),
      _user_limit(0),
      _users(0),
      _invite_only(false),
      _has_pass(false),
      _has_user_limit(false),
      _topic_restriction(false) {}

Channel::Channel(const Channel &src) {
  *this = src;
}

Channel::~Channel(void) {
  this->_clients.clear();
}

Channel &Channel::operator=(const Channel &src) {
  this->_name = src._name;
  this->_hostname = src._hostname;
  this->_topic = src._topic;
  this->_pass = src._pass;
  this->_user_limit = src._user_limit;
  this->_users = src._users;
  this->_clients = src._clients;
  this->_operator_clients = src._operator_clients;
  this->_invited_clients = src._invited_clients;
  this->_invite_only = src._invite_only;
  this->_has_pass = src._has_pass;
  this->_has_user_limit = src._has_user_limit;
  return *this;
}

// Member Functions

void Channel::join(Client *client) {
  this->_clients.push_back(client);
  this->broadcast(client, "JOIN", this->get_name(), "");
}

void Channel::quit(Client *client) {
  this->remove_channel_operator(client);
  this->broadcast(client, "QUIT", this->get_name(), "");
  this->leave(client);
}

void Channel::topic(Client *client, std::vector<std::string> params) {
  // Get Channel Topic
  if (params.size() == 1) {
    if (this->get_topic().empty())
      client->reply("331", this->get_name() + " " + ":No Topic Set.\r\n");
    else
      client->reply("332", this->get_name() + " " + this->get_topic());
  }
  // Set Channel Topic
  if (params.size() == 2) {
    std::string topic = params[1];
    this->set_topic(topic);
    this->broadcast(client, "TOPIC", this->get_name(), topic);
  }
}

void Channel::names(Client *client) {
  client->reply("353",
                "= " + this->get_name() + " :" + this->get_client_names());
  client->reply("366", this->get_name() + " :End of NAMES list");
}

void Channel::kick(Client *client, Client *target, std::string cause) {
  if (client != target && !is_channel_operator(client->get_nickname())) {
    client->reply("482",
                  this->get_name() + " " + ":You're not a channel operator");
    return;
  }
  std::string msg = target->get_nickname() + " :" + cause;
  this->broadcast(client, "KICK", this->get_name(), msg);
  this->leave(target);
}

void Channel::add_channel_operator(Client *client) {
  this->_operator_clients.push_back(client);
}

void Channel::remove_channel_operator(Client *client) {
  std::vector<Client *>::iterator it = std::find(
      this->_operator_clients.begin(), this->_operator_clients.end(), client);
  if (it != this->_operator_clients.end())
    this->_operator_clients.erase(it);
}

bool Channel::is_channel_operator(std::string nickname) {
  for (std::vector<Client *>::iterator it = this->_operator_clients.begin();
       it != this->_operator_clients.end(); it++)
    if ((*it)->get_nickname() == nickname)
      return true;
  return false;
}

void Channel::invite(Client *client) {
  this->_invited_clients.push_back(client);
}

void Channel::leave(Client *client) {
  remove_channel_operator(client);

  std::vector<Client *>::iterator it =
      std::find(this->_clients.begin(), this->_clients.end(), client);
  if (it != this->_clients.end()) {
    this->_clients.erase(it);
    this->_users--;
  }
}

// Set mode and unset mode are not finished yet
void Channel::set_mode(char mode,
                       Server &server,
                       std::vector<std::string> params,
                       Client *client,
                       std::string channel_name) {
  std::string message = "";
  Client *target;
  std::string target_name;
  std::string limit_param;
  int limit;

  switch (mode) {
    case 't':
      _topic_restriction = true;
      message = "+t";
      break;
    case 'i':
      _invite_only = true;
      message = "+i";
      break;
    case 'k':
      if (params.size() < 3) {
        server.sendResponse(
            client->get_socket(),
            ERR_NEEDMOREPARAMS(channel_name, server.getHostname()));
        break;
      }
      _pass = params[2];
      _has_pass = true;
      message = "+k " + _pass;
      break;
    case 'o':
      if (params.size() < 3) {
        server.sendResponse(
            client->get_socket(),
            ERR_NEEDMOREPARAMS(channel_name, server.getHostname()));
        break;
      }
      target_name = params[2];
      target = get_client(target_name, _clients);
      if (!this->has_client(target)) {
        server.sendResponse(client->get_socket(),
                            ERR_USERNOTINCHANNEL(target_name, channel_name,
                                                 server.getHostname()));
        break;
      }
      if (this->is_channel_operator(target_name)) {
        server.sendResponse(
            client->get_socket(),
            channel_name + " +o " + target_name + " :is already op");
      }
      add_channel_operator(target);
      message = "+o " + target_name;
      break;
    case 'l':
      if (params.size() < 3) {
        server.sendResponse(
            client->get_socket(),
            ERR_NEEDMOREPARAMS(channel_name, server.getHostname()));
        break;
      }
      limit_param = params[2];
      if (limit_param.find_first_not_of("0123456789") != std::string::npos) {
        server.sendResponse(client->get_socket(),
                            ERR_UNKNOWNMODE(channel_name));
        break;
      }
      // Check if limit is greater than 0
      limit = std::atoi(limit_param.c_str());
      if (limit < 1) {
        server.sendResponse(client->get_socket(),
                            ERR_UNKNOWNMODE(channel_name));
        break;
      }
      _user_limit = limit;
      _has_user_limit = true;
      message = "+l " + limit_param;
      break;
    default:
      server.sendResponse(client->get_socket(), ERR_UNKNOWNMODE(channel_name));
      break;
  }
  if (message.empty())
    return;
  this->broadcast(client, "MODE", this->get_name(), message);
}

void Channel::unset_mode(char mode,
                         Server &server,
                         std::vector<std::string> params,
                         Client *client,
                         std::string channel_name) {
  std::string message = "";
  Client *target;
  std::string target_name;
  std::string limit_param;

  switch (mode) {
    case 't':
      _topic_restriction = false;
      message = "-t";
      break;
    case 'i':
      _invite_only = false;
      message = "-i";
      break;
    case 'o':
      if (params.size() < 3) {
        server.sendResponse(
            client->get_socket(),
            ERR_NEEDMOREPARAMS(channel_name, server.getHostname()));
        break;
      }
      target_name = params[2];
      target = get_client(target_name, _clients);
      if (!this->has_client(target)) {
        server.sendResponse(client->get_socket(),
                            ERR_USERNOTINCHANNEL(target_name, channel_name,
                                                 server.getHostname()));
        break;
      }
      if (!this->is_channel_operator(target_name)) {
        server.sendResponse(
            client->get_socket(),
            ERR_CHANOPRIVSNEEDED(channel_name, server.getHostname()));
        break;
      }
      this->remove_channel_operator(target);
      message = "-o " + target_name;
      break;
    case 'k':
      _pass = "";
      _has_pass = false;
      message = "-k";
      break;
    case 'l':
      this->_has_user_limit = false;
      this->_user_limit = 0;
      message = "-l";
      break;
    default:
			server.sendResponse(client->get_socket(), ERR_UNKNOWNMODE(channel_name));
      break;
  }

  if (message.empty())
    return;
  this->broadcast(client, "MODE", this->get_name(), message);
  return;
}

void Channel::broadcast(Client *sender,
                        std::string command,
                        std::string target,
                        std::string message) {
  if (target.empty())
    target = this->get_name();
  for (std::vector<Client *>::iterator it = this->_clients.begin();
       it != this->_clients.end(); it++) {
    if (command == "PRIVMSG" && *it == sender)
      continue;
    (*it)->broadcast(sender, command, target, message);
  }
}

std::string Channel::get_name(void) const {
  return this->_name + ":" + this->_hostname;
}

std::string Channel::get_topic(void) const {
  if (this->_topic.empty())
    return this->_topic;
  else
    return ":" + this->_topic;
}

std::string Channel::get_key(void) const {
  return this->_pass;
}

// Not Implemented
std::string Channel::get_modes(void) {
  std::string modes = "";
  return modes;
}

int Channel::get_user_limit(void) const {
  return this->_user_limit;
}

int Channel::get_user_quantity(void) const {
  return this->_users;
}

Client *Channel::get_client(std::string nickname,
                            std::vector<Client *> &clients) {
  for (std::vector<Client *>::const_iterator it = clients.begin();
       it != clients.end(); it++)
    if ((*it)->get_nickname() == nickname)
      return *it;
  return NULL;
}

std::vector<Client *> Channel::get_clients(void) {
  return this->_clients;
}

std::vector<Client *> Channel::get_chanop_clients(void) {
  return this->_operator_clients;
}

std::string Channel::get_client_names(void) {
  std::string names;
  for (std::vector<Client *>::iterator it = this->_clients.begin();
       it != this->_clients.end(); it++) {
    names += is_channel_operator((*it)->get_nickname()) ? "@" : "";
    names += (*it)->get_nickname() + " ";
  }
  return names;
}

std::string Channel::get_chanop_names(void) {
  std::string names;
  for (std::vector<Client *>::const_iterator it =
           this->_operator_clients.begin();
       it != this->_operator_clients.end(); it++)
    names += (*it)->get_nickname() + " ";
  return names;
}

std::string Channel::get_invited_names(void) {
  std::string names;
  for (std::vector<Client *>::const_iterator it =
           this->_invited_clients.begin();
       it != this->_invited_clients.end(); it++) {
    names += (*it)->get_nickname() + " ";
  }
  return names;
}

std::string Channel::get_client_info(Client *client, Channel *channel) {
  std::string info = "";
  info += client->get_nickname();
  info += " " + client->get_username();
  info += " " + client->get_hostname();
  info += " " + client->get_hostname();
  info += " " + client->get_nickname();
  info += " H";
  if (channel->is_channel_operator(client->get_nickname()))
    info += "@";
  info += " :" + client->get_realname();
  return info;
}

bool Channel::get_topic_restriction(void) const {
  return _topic_restriction;
}

bool Channel::get_invite_only(void) const {
  return _invite_only;
}

bool Channel::has_key(void) const {
  return _has_pass;
}

bool Channel::has_user_limit(void) const {
  return _has_user_limit;
}

bool Channel::has_client(Client *client) {
  for (std::vector<Client *>::iterator it = this->_clients.begin();
       it != this->_clients.end(); it++)
    if ((*it)->get_nickname() == client->get_nickname())
      return true;
  return false;
}

void Channel::set_topic(const std::string topic) {
  this->_topic = topic;
}

void Channel::increase_user_quantity(void) {
  this->_users++;
}