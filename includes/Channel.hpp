#ifndef CHANNELL_HPP
#define CHANNELL_HPP

#include "Server.hpp"
#include "Client.hpp"

class Server;

class Channel {
 private:
	Server *_server;
  std::string _name;
  std::string _hostname;
  std::string _topic;
  std::string _pass;

  size_t _user_limit;
  int _users;

  std::vector<Client *> _clients;
  std::vector<Client *> _operator_clients;
  std::vector<Client *> _invited_clients;

  bool _invite_only;
  bool _has_pass;
  bool _has_user_limit;
  bool _topic_restriction;

 public:
  // Constructors and Destructors
  Channel(std::string name, Server &server);
  Channel(const Channel &src);
  ~Channel(void);

  Channel &operator=(const Channel &src);

  // Member Functions
  void join(Client *client);
	void part(Client *client, std::string cause);
  void quit(Client *client);
  void topic(Server &server, Client *client, std::vector<std::string> params);
  void names(Server &server, Client *client);
  void kick(Server &server, Client *client, Client *target, std::string cause);

  void add_channel_operator(Client *client);
  void remove_channel_operator(Client *client);
  bool is_channel_operator(std::string nickname);

  void invite(Client *client);
  void leave(Client *client);
  void set_mode(char mode,
                Server &server,
                std::vector<std::string> params,
                Client *client,
                std::string channel_name);
  void unset_mode(char mode,
                  Server &server,
                  std::vector<std::string> params,
                  Client *client,
                  std::string channel_name);

  void broadcast(Client *sender,
                 std::string command,
                 std::string target,
                 std::string message);

  // Getters and Setters
  std::string get_name(void) const;
  std::string get_topic(void) const;
  std::string get_key(void) const;
  std::string get_modes(void);
  int get_user_limit(void) const;
  int get_user_quantity(void) const;
  Client *get_client(std::string nickname, std::vector<Client *> &clients);
  std::vector<Client *> get_clients(void);
  std::vector<Client *> get_chanop_clients(void);
  std::string get_client_names(void);
  std::string get_chanop_names(void);
  std::string get_invited_names(void);
  std::string get_client_info(Client *client, Channel *channel);
  bool get_topic_restriction(void) const;
  bool get_invite_only(void) const;
  bool has_key(void) const;
  bool has_user_limit(void) const;
  bool has_client(Client *client);
  bool isUserOnChannel(std::string nickname);
  bool isTopicRestrictedToOperators();

  // Setters
  void set_topic(const std::string topic);
  void increase_user_quantity(void);
};

#endif
