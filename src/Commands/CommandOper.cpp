#include "CommandOper.hpp"
#include <string>
#include "Channel.hpp"
#include "Server.hpp"

CommandOper::CommandOper() {}
CommandOper::~CommandOper() {}
void CommandOper::execute(int &clientSocket,
                          Client *client,
                          Server *server,
                          std::vector<std::string> *params) {
  if (client->is_authenticated() && client->is_registered()) {
    if (params->size() < 1) {
      server->sendResponse(clientSocket,
                           ERR_NEEDMOREPARAMS("OPER", server->get_hostname()));
      return;
    }
    std::string tar_name = (*params)[0];
    Client *tar = server->get_client_by_nickname(tar_name);
    if (tar) {
      if (!tar->is_registered()) {
        server->sendResponse(
            client->get_socket(),
            ERR_NOTREGISTERED(server->get_hostname(), tar_name));
        return;
      }
      if (tar->is_operator()) {
        server->sendResponse(client->get_socket(),
                             ":" + tar_name + "is already an IRC operator\r\n");
        return;
      }
      // If client is an operator, it can assign operator to another
      // client without password
      if (params->size() == 1) {
        if (!client->is_operator()) {
          server->sendResponse(clientSocket,
                               ERR_NOPRIVILEGES(server->get_hostname()));
          return;
        } else
          tar->set_operator(server->get_oper_password());
        // Otherwise, check if the password is correct
      } else
        tar->set_operator((*params)[1]);
      if (tar->is_operator()) {
        std::string tar_is = tar_name + " is";
        if (client->get_nickname() != tar_name)
          server->sendResponse(clientSocket, RPL_YOUREOPER(tar_is));
        std::string you = "You are";  // IRRSI doesn't show the 'You' for some
                                      // reason, only displays 'are' ??
        server->sendResponse(tar->get_socket(), RPL_YOUREOPER(you));
      } else
        server->sendResponse(clientSocket,
                             ERR_PASSWDMISMATCH(server->get_hostname()));
    } else
      server->sendResponse(clientSocket,
                           ERR_NOSUCHNICK(tar_name, server->get_hostname()));
  } else
    server->sendResponse(
        clientSocket,
        ERR_NOTREGISTERED(server->get_hostname(), client->get_nickname()));
}
