#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

const std::string g_oper_password = "oper";

class Server;

class Client {
 private:
  Server *_server;
  int _socket;
  bool _operator;
  bool _authenticated;
  bool _registered;
	bool _disconnected;
  std::string _server_hostname;
  std::string _nickname;
  std::string _username;
  std::string _realname;
  std::string currentMessage;
  std::string _hostname;
  std::string _servername;

  void processMessage(const char *msg);


 public:
  Client(int fd, Server &server, std::string ip);
  ~Client();
  bool isMessageReady();

  std::string get_EntireMessage();
  std::string get_nickname(void) const;
  std::string get_realname(void) const;
  std::string get_username(void) const;
  std::string get_hostname(void) const;
  std::string get_servername(void) const;
  std::string get_buffer(void) const;
  int get_socket(void) const;
  bool is_operator(void) const;
  bool is_authenticated(void) const;
  bool is_registered(void) const;
	bool is_disconnected(void) const;

	void set_disconnected(void);
  void set_nickname(std::string nickname);
  void set_realname(std::string realname);
  void set_username(std::string username);
  void set_servername(std::string servername);
  void set_hostname(std::string hostname);
	void set_buffer(std::string buffer);
  void set_operator(std::string oper_password);
  void unset_operator(void);
	void clear_buffer(void);

  std::deque<std::string> pendingWrite;

  void authenticate(void);
  void register_client(void);
  std::string generatePrefix(void);

  // User data
  void reply(std::string code, std::string msg);
  void broadcast(Client *sender,
                 std::string command,
                 std::string target,
                 std::string message);

};

#endif
