#include "CommandInvite.hpp"
#include "Channel.hpp"
#include "Server.hpp"

CommandInvite::CommandInvite() {}
CommandInvite::~CommandInvite() {}
void CommandInvite::execute(int &clientSocket,
                            Client *client,
                            Server *server,
                            std::vector<std::string> *params) {
  if (params->size() <= 1)  // not enough parameters
  {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("INVITE", client->get_hostname()));
    return;
  }

  if (client->is_authenticated() == false)  // client not authenticated
  {
    server->sendResponse(
        clientSocket,
        ERR_NOTREGISTERED(client->get_hostname(), client->get_nickname()));
    return;
  }

  // if the invited user is already on the channel
  if (server->get_channel(params->at(1)) != NULL &&
      server->get_channel(params->at(1))->isUserOnChannel(params->at(0)) ==
          true) {
    server->sendResponse(clientSocket,
                         ERR_USERONCHANNEL(params->at(0), params->at(1),
                                           client->get_hostname()));
    return;
  }

  if (server->get_client_by_nickname(params->at(0)) ==
      NULL)  // if nickname does not exist
  {
    server->sendResponse(clientSocket,
                         ERR_NOSUCHNICK(params->at(0), client->get_hostname()));
    return;
  }

  // if client is not on the channel he is inviting to
  if (server->get_channel(params->at(1)) != NULL &&
      server->get_channel(params->at(1))->isUserOnChannel(params->at(0)) ==
          true) {
    server->sendResponse(
        clientSocket, ERR_NOTONCHANNEL(params->at(1), client->get_hostname()));
    return;
  }

  // client is not an operator and this is required
  if (server->get_channel(params->at(1)) != NULL &&
      (server->get_channel(params->at(1))->get_invite_only() == true ||
      server->get_channel(params->at(1))->is_channel_operator(client->get_nickname()) ==
          false)) {
    server->sendResponse(
        clientSocket,
        ERR_CHANOPRIVSNEEDED(params->at(1), client->get_hostname()));
    return;
  }

  if (server->get_channel(params->at(1)) != NULL) {
    server->get_channel(params->at(1))
        ->invite(server->get_client_by_nickname(params->at(0)));

    // message to client who is sending the invitation
    server->sendResponse(
        clientSocket,
        RPL_INVITING(params->at(1), params->at(0), client->get_hostname()));

    // message to the client who is receiving the invitation
    server->sendResponse(
        server->get_client_by_nickname(params->at(0))->get_socket(),
        RPL_INVITING(params->at(1), params->at(0), client->get_hostname()));
  }
}

/*
Invite message

      Command: INVITE
   Parameters: <nickname> <channel>

   The INVITE message is used to invite users to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  To invite a user to a channel which is invite only (MODE
   +i), the client sending the invite must be recognised as being a
   channel operator on the given channel.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_USERONCHANNEL
           ERR_CHANOPRIVSNEEDED
           RPL_INVITING

   Examples:

   :Angel INVITE Wiz #Dust         ; User Angel inviting WiZ to channel #Dust

   INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to #Twilight_zone

*/
