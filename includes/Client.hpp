#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

class Client {
 public:
  Client(int fd, std::string ip);
  ~Client();
  void messageHandler(char msg[]);
  bool isMessageReady();
  std::string getEntireMessage();
  std::string get_nickname(void) const;
  std::string get_realname(void) const;
  std::string get_username(void) const;
  std::string get_hostname(void) const;
  std::deque<std::string> pendingWrite;
  void authenticate();
  bool getAuthentication();
  void reply(std::string code, std::string msg);
  void broadcast(Client *sender,
                 std::string command,
                 std::string target,
                 std::string message);

 private:
  int _socket;
  std::string _server_hostname;
  std::string _nickname;
  std::string _username;
  std::string _realname;
  std::string fullMessage;
  std::string currentMessage;

  bool isAuthenticated;

  void processMessage(const char *msg);
};

#endif
