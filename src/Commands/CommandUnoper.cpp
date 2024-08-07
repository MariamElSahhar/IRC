#include "CommandUnoper.hpp"
#include "Channel.hpp"
#include "ErrorCodes.hpp"
#include "Server.hpp"

CommandUnOper::CommandUnOper() {}
CommandUnOper::~CommandUnOper() {}
void CommandUnOper::execute(int &clientSocket,
                            Client *client,
                            Server *server,
                            std::vector<std::string> *params) {
  if (client->is_authenticated() && client->is_registered()) {
    // Check if message has enough parameters
    if (params->size() < 1) {
      server->sendResponse(
          clientSocket, ERR_NEEDMOREPARAMS("UNOPER", server->get_hostname()));
      return;
    }
    // Check if the client has the necessary permissions
    if (!client->is_operator()) {
      server->sendResponse(clientSocket,
                           ERR_NOPRIVILEGES(server->get_hostname()));
      return;
    }
    std::string tar_name = (*params)[0];
    Client *tar = server->get_client_by_nickname(tar_name);
    if (tar) {
      if (!tar->is_operator()) {
        server->sendResponse(clientSocket,
                             ERR_NOPRIVILEGES(server->get_hostname()));
        return;
      }
      tar->unset_operator();
      if (client->get_nickname() != tar_name)
        server->sendResponse(
            clientSocket,
            ERR_CHANOPRIVSNEEDED(server->get_hostname(), tar_name));
      server->sendResponse(
          tar->get_socket(),
          ERR_CHANOPRIVSNEEDED(server->get_hostname(), tar_name));
    } else
      server->sendResponse(clientSocket,
                           ERR_NOSUCHNICK(tar_name, server->get_hostname()));
  } else
    server->sendResponse(
        clientSocket,
        ERR_NOTREGISTERED(server->get_hostname(), client->get_nickname()));
}
