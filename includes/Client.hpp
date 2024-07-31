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
 public:
  Client(int fd, Server &server, std::string ip);
  ~Client();
  void messageHandler(char msg[]);
  bool isMessageReady();

  std::string get_EntireMessage();
  std::string get_nickname(void) const;
  std::string get_realname(void) const;
  std::string get_username(void) const;
  std::string get_hostname(void) const;
  int get_socket(void) const;
  bool get_Authentication(void) const;
  bool is_operator(void) const;
  bool is_registered(void) const;

  void set_nickname(std::string nickname);
  void set_realname(std::string realname);
  void set_username(std::string username);
  void set_operator(std::string oper_password);
  void unset_operator(void);

  std::deque<std::string> pendingWrite;

  void authenticate();
  void register_client(void);
  void registerClient();
  bool getRegistration();

  // User data
  void reply(std::string code, std::string msg);
  void broadcast(Client *sender,
                 std::string command,
                 std::string target,
                 std::string message);
  void setUsername(std::string username);
  std::string getUsername();
  void setRealname(std::string realname);
  std::string getRealname();
  void setHostname(std::string hostname);
  std::string getHostname();
  void setServername(std::string servername);
  std::string getServername();

 private:
  Server *_server;
  int _socket;
  bool _operator;
  bool _authenticated;
  bool _registered;
  std::string _server_hostname;
  std::string _nickname;
  std::string _username;
  std::string _realname;
  std::string fullMessage;
  std::string currentMessage;

  std::string _hostname;
  std::string _servername;

  void processMessage(const char *msg);
};

#endif
