#include "CommandPass.hpp"
#include "Server.hpp"

CommandPass::CommandPass() {}
CommandPass::~CommandPass() {}

void CommandPass::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *param) {
  // check if the format of the password is valid:
  if (param->size() < 1) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("PASS", server->getHostname()));
    return;
  }

  // check if it is not in use yet (if it was already authenticated):
  if (client->get_Authentication() == true) {
    server->sendResponse(clientSocket,
                         ERR_ALREADYREGISTERED(server->getHostname()));
    return;
  }

  // check if the password provided match with the one provided as av[2];
  if ((*param)[0] == server->getPassword())
    client->authenticate();
  else
    server->sendResponse(clientSocket,
                         ERR_PASSWDMISMATCH(server->getHostname()));
}
