#ifndef SERVER_HPP
#define SERVER_HPP

#include "CommandFactory.hpp"
#include "IrcClients.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <utility>
#include <map>
#include <csignal>


#define MAX_CON 100
#define MAX_BUF 512
#define HOSTNAME "127.0.0.1"
#define IP_BIND "127.0.0.1"


class CommandFactory;

class Server
{
	public:
		Server(int port, std::string password, IrcClients *ircClients, CommandFactory *commandFactory);
		~Server();

		void start(); // do all to start server
		void createSocket();
		void waitConnections();
		void acceptConnection();
		void readMessage(int indexFd);
		void sendResponse(int clientSocket, std::string msg);
		void cleanUp();

	private:
		int									portNb;
		std::string							password;
		std::string							ipAddress;
		std::string							hostName;
		int									socketNb;
		std::vector<pollfd>					pollFdVector;
		IrcClients 							*ircClients;
		CommandFactory 						*commandFactory;


		Server();
		Server(Server const &);
		Server &operator=(Server const &);
};


#endif
