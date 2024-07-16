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
  void setNickname(std::string nickname);
  std::string getNickname();

 private:
  std::string fullMessage;
  std::string currentMessage;
  std::string _nickname;

  bool isAuthenticated;

  void processMessage(const char *msg);
};

#endif
