#include "CommandTopic.hpp"
#include "Channel.hpp"
#include "ErrorCodes.hpp"
#include "Server.hpp"

CommandTopic::CommandTopic() {}
CommandTopic::~CommandTopic() {}
void CommandTopic::execute(int &clientSocket,
                           Client *client,
                           Server *server,
                           std::vector<std::string> *params) {
  std::string topicName;
  std::vector<Client *> channelClients;

  // Input validation:
  // syntax: TOPIC <channel>             to view the channel topic name
  // syntax: TOPIC <channel> <:topic>    to change the topic name
  if (params->size() < 1 || params->size() > 2) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("TOPIC", server->get_hostname()));
    return;
  }

  // Check if the client is authenticated:
  if (!client->is_authenticated()) {
    server->sendResponse(
        clientSocket,
        ERR_NOTREGISTERED(client->get_hostname(), client->get_nickname()));
    return;
  }

  // Check if the channel exists
  if (server->get_channel(params->at(0)) == NULL)  // Channel does not exist
  {
    server->sendResponse(
        clientSocket, ERR_NOSUCHCHANNEL(params->at(0), server->get_hostname()));
    return;
  }

  if (params->size() == 1)  // view channel name
  {
    Channel *channel = server->get_channel(params->at(0));
    topicName = channel->get_topic();
    if (!topicName.empty())  // will show the topic name
    {
      server->sendResponse(
          clientSocket,
          RPL_TOPIC(client->generatePrefix(), client->get_nickname(),
                    params->at(0), server->get_hostname()));
    } else  // will show ":No Topic Set"
    {
      server->sendResponse(clientSocket,
                           RPL_NOTOPIC(client->generatePrefix(),
                                       client->get_nickname(), params->at(0)));
    }
    return;
  }
  Channel *channel = server->get_channel(params->at(0));
  if (params->size() == 2)  // change channel name
  {
    if (channel->isUserOnChannel(client->get_nickname()) == false) {
      server->sendResponse(
          clientSocket,
          ERR_NOTONCHANNEL(params->at(0), server->get_hostname()));
      return;
    }
    // if change the topic name is restricted to operator:
    if (channel->isTopicRestrictedToOperators() == true) {
      if (channel->is_channel_operator(client->get_nickname()) == false) {
        server->sendResponse(
            clientSocket,
            ERR_CHANOPRIVSNEEDED(params->at(0), server->get_hostname()));
        return;
      }
    }
    if (params->at(1).length() > 0 && params->at(1).length() < MAX_TOPIC_LEN) {
      channel->set_topic(params->at(1));
    } else
      channel->set_topic("");
  }

  channelClients = channel->get_clients();
  for (size_t i = 0; i < channelClients.size(); i++) {
    server->sendResponse(
        channelClients[i]->get_socket(),
        RPL_TOPIC(client->generatePrefix(), client->get_nickname(),
                  params->at(1), server->get_hostname()));
  }
}

/*
Command: TOPIC
   Parameters: <channel> [ <topic> ]

   The TOPIC command is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if this action is allowed for the user
   requesting it.
   IMPORTANT: If the <topic> parameter is an empty string, the
   topic for that channel will be removed.

   After changing the TOPIC name all user get a notification msg.
*/
