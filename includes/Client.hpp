#ifndef CLIENT_HPP
#define CLIENT_HPP

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
  std::deque<std::string> pendingWrite;
  void authenticate();
  bool getAuthentication();

  // User data
  void setNickname(std::string nickname);
  std::string getNickname();
  void setUsername(std::string username);
  std::string getUsername();
  void setRealname(std::string realname);
  std::string getRealname();

 private:
  std::string fullMessage;
  std::string currentMessage;
  std::string _nickname;
  std::string _realname;
  std::string _username;
  std::string _hostname;
  std::string _servername;

  bool isAuthenticated;

  void processMessage(const char *msg);
};

#endif
