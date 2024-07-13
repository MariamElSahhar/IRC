#ifndef COMMANDPRIVMSG_HPP
#define COMMANDPRIVMSG_HPP

#include "ICommand.hpp"

class CommandPrivMsg : public ICommand {
	public:
		CommandPrivMsg();
		virtual ~CommandPrivMsg();
		void execute(int &clientSocket, Client *client, Server *server, std::string payload);
};

#endif