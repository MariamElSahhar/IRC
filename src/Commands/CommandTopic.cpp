#include "CommandTopic.hpp"
#include "Server.hpp"

CommandTopic::CommandTopic() {}
CommandTopic::~CommandTopic() {}
void CommandTopic::execute(int &clientSocket,
                           Client *client,
                           Server *server,
                           std::vector<std::string> *params) {}

std::string topicName;

// Input validation:
// syntax: TOPIC <channel>             to view the channel topic name
// syntax: TOPIC <channel> <:topic>    to change the topic name
if (param->size() < 1 || param->size() > 2) {
    server->sendResponse(clientSocket,
                         ERR_NEEDMOREPARAMS("TOPIC", server->get_hostname()));
    return;
  }

// Check if the client is authenticated:
if (!client->is_authenticated()) {
    server->sendResponse(clientSocket,
                         ERR_NOTREGISTERED(server->get_hostname()));
    return;
}

// Check if the channel exists
if (server->get_channel(params[0]) == NULL) // Channel does not exist
{
    server->sendResponse(clientSocket,
                         ERR_NOSUCHCHANNEL(param[0], server->get_hostname()));
    return;
}

if (param.size() == 1) // view channel name
{
    topicName = server->get_channel(params[0])->get_topic();
    if (!topicName.empty())
    {
        server->sendResponse(clientSocket,
                        RPL_TOPIC(prefix, param[0], server->get_hostname()));
    }

    return;
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
*/