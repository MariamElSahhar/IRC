/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbalbino <pbalbino@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 15:27:02 by pbalbino          #+#    #+#             */
/*   Updated: 2024/07/11 15:35:22 by pbalbino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "CommandFactory.hpp"
#include "IrcClients.hpp"
#include "Server.hpp"

// Ports go from 0 to 65535 (5 digits - length)
// Ports below 1024 are reserved
bool checkPortNumber(std::string port) {
  if (port.find_first_not_of("0123456789") != std::string::npos ||
      port.length() > 5) {
    std::cerr << "Ports go from 1025 to 65535 and accepts only digits! Ports "
                 "below 1024 are reserved."
              << std::endl;
    return (false);
  }

  int portNb = atoi(port.c_str());
  if (portNb < 1024 || portNb > 65535) {
    std::cerr << "Ports go from 1025 to 65535 and accepts only digits! Ports "
                 "below 1024 are reserved."
              << std::endl;
    return (false);
  }
  return (true);
}

// Password rules:
// Must be between 4 and 32 characters
// Must contain only alphanumeric characters
// PB: I did not found anything on the IRC documentation about this, so I guess
// this is up to us to decide
bool isValidPassword(const std::string &password) {
  if (password.length() < 4 || password.length() > 32) {
    std::cerr << "Password needs to be between 6 and 32 letters long."
              << std::endl;
    return (false);
  }

  for (size_t i = 0; i < password.length(); ++i) {
    if (!isalnum(password[i])) {
      std::cerr << "Password can only contain alphanumeric letters."
                << std::endl;
      return (false);
    }
  }
  return true;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Incorrect number of arguments. Please write: ircserv <port> "
                 "<password>."
              << std::endl;
    return (EXIT_FAILURE);
  }
  std::string port = argv[1];
  std::string password = argv[2];

  if (checkPortNumber(port) == true && isValidPassword(password) == true) {
    int portNumber = atoi(port.c_str());
    IrcClients *ircClients =
        new IrcClients();  // creates an object of the Irc Clients, which is
                           // responsible to manage all the connected clientes
    CommandFactory *commandFactory =
        new CommandFactory();  // creates an object of the Command Factory,
                               // which is reponsible to create command objects
                               // depending on the command type (PASS, MODE,
                               // etc)
    Server server(portNumber, password, ircClients,
                  commandFactory);  // this is the most important class, which
                                    // is reponsible for opening the socket and
                                    // waiting for connections
    server.start();  // here we will start the server and keep waiting and
                     // processing conections, on an infinite loop

    delete commandFactory;
    delete ircClients;
    return (EXIT_SUCCESS);
  } else
    return (EXIT_FAILURE);
}

// From here I recommend going to start method "server.start();"
// After make, run "./ircserv 9999 mypassword" for example, it will start the
// server and in another terminal, run "nc 127.0.0.1 9999" (same port) and type
// "PASS mypassword"(same password)
