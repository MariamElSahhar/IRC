#include "CommandPass.hpp"
#include "ErrorCodes.hpp"
#include "Server.hpp"

CommandPass::CommandPass() {}
CommandPass::~CommandPass() {}

std::vector<std::string> split(const std::string &str, char delimiter) {
  std::vector<std::string> result;
  std::string::size_type start = 0;
  std::string::size_type end;

  while ((end = str.find(delimiter, start)) != std::string::npos) {
    result.push_back(str.substr(start, end - start));
    start = end + 1;
  }

  // Add the last substring
  result.push_back(str.substr(start));

  return result;
}

void CommandPass::execute(int &clientSocket, Client *client, Server *server,
                          std::string payload) {
  std::cout << "COMMAND: PASS - payload:" << payload << std::endl;
  std::vector<std::string> strings = split(payload, ' ');

  // check if the format of the password is valid:
  if (strings.size() > 1) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("PASS", server->getHostname()));
    return;
  }

  // check if it is not in use yet (if it was already authenticated):
  if (client->getAuthentication() == true) {
    server->sendResponse(clientSocket,
                         ERR_ALREADYREGISTERED(server->getHostname()));
    return;
  }

  // check if the password provided match with the one provided as av[2];
  if (strings[0] == server->getPassword()) {
    client->authenticate();
  } else
    server->sendResponse(clientSocket,
                         ERR_PASSWDMISMATCH(server->getHostname()));
}
