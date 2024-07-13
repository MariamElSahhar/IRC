#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <deque>
#include <string>

class Client {
public:
  Client(int fd, std::string ip);
  ~Client();
  void messageHandler(char msg[]);
  bool isMessageReady();
  std::string getEntireMessage();
  std::deque<std::string> pendingWrite;

private:
  std::string fullMessage;
  std::string currentMessage;

  void processMessage(const char *msg);
};

#endif