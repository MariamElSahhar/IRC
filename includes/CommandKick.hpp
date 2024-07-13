#ifndef COMMANDKICK_HPP
#define COMMANDKICK_HPP

#include "ICommand.hpp"

class CommandKick : public ICommand {
	public:
		CommandKick();
		virtual ~CommandKick();
		void execute(int &clientSocket, Client *client, Server *server, std::string payload);
};

#endif