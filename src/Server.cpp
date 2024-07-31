#include "Server.hpp"
#include "Channel.hpp"
#include "CommandFactory.hpp"
#include "IrcClients.hpp"
#include "IrcCommandParser.hpp"

bool running = true;
void signalHandler(int sigNum) {
  (void)sigNum;
  running = false;
}

Server::Server() {}

Server::Server(Server const &) {}

Server &Server::operator=(Server const &) {
  return *this;
}

Server::Server(int port,
               std::string password,
               IrcClients *ircClients,
               CommandFactory *commandFactory)
    : portNb(port),
      socketNb(-1),
      password(password),
      ipAddress(IP_BIND),
      hostName(HOSTNAME),
      pollFdVector(),
      ircClients(ircClients),
      commandFactory(commandFactory) {
  std::cout << std::endl << CYAN "Starting IRC Server ..." RESET << std::endl;
}

Server::~Server() {
  cleanUp();
}

void Server::start() {
  int fd = 0;

  try {
    createSocket(fd);
  } catch (std::exception &e) {
    std::cerr << RED << e.what() << RESET << std::endl;
    if (fd)
      close(fd);
    exit(EXIT_FAILURE);
  }
  try {
    waitConnections();
  } catch (std::exception &e) {
    cleanUp();
    std::cerr << std::endl << RED << e.what() << RESET << std::endl;
  }
}

std::string Server::getPassword() {
  return (this->password);
}

std::string Server::getHostname() {
  return (this->hostName);
}

void Server::createSocket(int fd) {
  // Creating the Socket: The socket is created using the socket()
  // function with parameters IPv4, TCP, and 0 (default protocol for TCP).
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
    throw std::runtime_error("Error creating socket.");

  // The setsockopt() function is called with the parameters:
  // - SOL_SOCKET: The socket level.
  // - SO_REUSEADDR: The socket option to reuse the local address.
  // - &enable: A pointer to the value to set the option to (1).
  int enable = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
    throw std::runtime_error("Error Socket was not cleared.");

  // The fcntl() function is used to set the fd (socket) to non-blocking mode
  // using the O_NONBLOCK flag.
  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    throw std::runtime_error("Error setting socket to non-blocking mode.");

  // Bind the Socket: The socket is bound to the configured address and port
  // using the bind() function.
  sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));  // Clear the struct
  serverAddr.sin_family =
      AF_INET;  // We use AF_INET to match with the socket FD (IPv4)
  serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
  serverAddr.sin_port =
      htons(portNb);  // htons is a system conversion/manipulation
  if (bind(fd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    throw std::runtime_error("Error while binding socket");

	// Listen for Connections: The socket is set to listen for incoming
	// connections using the listen() function, with a backlog of MAXCOUNT pending
	// connections. If our program was blocking, this function would block/suspend the
	// thread. In our case we are using non blocking sockets this will not block.
	// This means that the treatment of the connections will be made by poll.
	if (listen(fd, MAX_CON) < 0)
    throw std::runtime_error("Error listening on socket.");
  socketNb = fd;
}

void Server::waitConnections() {
  // Initializing pollfd struct and add server socket to the list
  pollfd serverPollfd;
  serverPollfd.fd = socketNb;
  serverPollfd.events = POLLIN;  // POLLIN: There is data to read.
  serverPollfd.revents = 0;
  pollFdVector.push_back(serverPollfd);

  std::string msg;
  int readStatus = 0;

  std::cout << std::endl << YELLOW "Waiting for clients ..." RESET << std::endl;
  signal(SIGINT, signalHandler);

  while (true) {
    if (running == false)
      throw std::runtime_error("Server stopped.");
    // int 5 int the ft below refers to the timeout argument that specifies the
    // number of milliseconds that poll should block waiting for a file
    // descriptor to become ready.
    // The call will block until either:
    // •  a file descriptor becomes ready;
    // •  the call is interrupted by a signal handler;
    // •  the timeout expires.
    if (poll(pollFdVector.data(), pollFdVector.size(), 5) < 0)
      throw std::runtime_error("Server stopped.");
    for (unsigned int i = 0; i < pollFdVector.size(); i++) {
      if (pollFdVector[i].revents & POLLIN)  // check all sockets
      {
        if (pollFdVector[i].fd == socketNb)  // checks info received from socket
          acceptConnection();
        else  // the message is only read if the connection is acceptable
          readStatus = readMessage(i);
      }
      if (readStatus == -1) {
        readStatus = 0;
        continue;
      }
		// verify if write/send a message is possible. POLLOUT:  Writing is now possible, though a
		// write larger than the available space in a socket or pipe will still block (unless O_NONBLOCK is set).
		// Each client its own "pendingWrite", since we are dealing with a non blocking scenario
		// "front" takes the oldest message of the client and after writing it, pop_front remove it from the "queue"
      if (pollFdVector[i].revents & POLLOUT) {
        Client *client = ircClients->getClient(pollFdVector[i].fd);
        if (client != NULL && !client->pendingWrite.empty()) {
          msg = client->pendingWrite.front();
          send(pollFdVector[i].fd, msg.c_str(), msg.size(), 0);
          client->pendingWrite.pop_front();
        }
      }
    }
  }
}

void Server::sendResponse(int clientSocket, std::string msg) {
  Client *client = ircClients->getClient(clientSocket);
  if (client != NULL)
    client->pendingWrite.push_back(msg);
}

Client *Server::getClientByNickname(const std::string &nickname) {
  return (ircClients->getClientByNickname(nickname));
}

void Server::acceptConnection() {
  // create info for new client and new socket
  sockaddr_in clientAddr;
  memset(&clientAddr, 0, sizeof(clientAddr));
  socklen_t clientSize = sizeof(clientAddr);

  int clientSocket = accept(socketNb, (sockaddr *)&clientAddr, &clientSize);
  if (clientSocket < 0) {
    std::cerr << "Connection can not be established with new client"
              << std::endl;
    return;
  }

  // creation of the poll
  pollfd clientPollfd;
  clientPollfd.fd = clientSocket;
  clientPollfd.events = POLLIN | POLLOUT;
  clientPollfd.revents = 0;
  pollFdVector.push_back(clientPollfd);

  // add new client to ClientArray
  std::string ipClient = inet_ntoa(clientAddr.sin_addr);
  ircClients->createClient(clientSocket, ipClient);

  std::cout << GREEN "Client successfully connected on socket fd: " RESET
            << clientSocket << std::endl;
  sendResponse(clientSocket,
               "Registeration is mandatory to use the server -> "
               "use: PASS, NICK and USER\r\n");
}

int Server::readMessage(int i) {
  int clientFd = pollFdVector[i].fd;
  char buffer[MAX_BUF];
  memset(buffer, 0, sizeof(buffer));
  int bytesRecv = recv(clientFd, buffer, sizeof(buffer), 0);
  if (bytesRecv <= 0) {
    if (bytesRecv == 0)  // only when close terminal
      std::cerr << YELLOW "Client disconnected from socket fd: " RESET
                << clientFd << std::endl;
    else
      std::cerr << "Connection Error!" << std::endl;

    //  deleting client that disconnected
    if (!ircClients->removeClient(clientFd)) {
      std::cerr << "Failed to remove client!" << std::endl;
    }
    std::vector<pollfd>::iterator it2;
    for (it2 = pollFdVector.begin(); it2 != pollFdVector.end(); it2++) {
      if (it2->fd == clientFd) {
        pollFdVector.erase(it2);
        break;
      }
    }
    return (-1);
  }
  std::cout << "Received: " << bytesRecv
            << " bytes Raw msg:" << std::string(buffer) << std::endl;
  Client *currentClient = ircClients->getClient(clientFd);
  if (currentClient != NULL) {
    currentClient->messageHandler(buffer);
    if (currentClient->isMessageReady()) {
      std::cout << "Message ready to be processed: "
                << currentClient->get_EntireMessage() << std::endl;
      IrcCommandParser parser =
          IrcCommandParser(std::string(currentClient->get_EntireMessage()));
      CommandType commandType = parser.getMessageType();
      if (commandType != INVALID) {
        std::cout << "commandType " << commandType << std::endl;
        ICommand *command = commandFactory->createCommand(commandType);
        std::vector<std::string> params = parser.getParams();
        command->execute(clientFd, currentClient, this, &params);
        delete command;
      } else
        std::cerr << "Invalid message received!" << std::endl;
    }
  } else
    std::cerr << "Client not found!" << std::endl;
  return 0;
}

void Server::add_channel(Channel *channel) {
  _channels.push_back(channel);
}

Channel *Server::get_channel(std::string name) {
  // if name doesn't end in ":hostname", append ":hostname"
  if (name.find(":") == std::string::npos)
    name += ":" + hostName;
  for (size_t i = 0; i < _channels.size(); i++)
    if (_channels[i]->get_name() == name)
      return _channels[i];
  return NULL;
}

void Server::cleanUp() {
  // close the fds
  if (!pollFdVector.empty()) {
    for (unsigned int i = 0; i < pollFdVector.size(); i++) {
      if (pollFdVector[i].fd > 0) {
        close(pollFdVector[i].fd);
        pollFdVector.erase(pollFdVector.begin() + i);
      }
    }
    pollFdVector.clear();
  }
}
