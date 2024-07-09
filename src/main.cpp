#include "Server.hpp"
#include "Client.hpp"


// Ports go from 0 to 65535 (5 digits - length)
// Ports below 1024 are reserved
bool checkPortNumber(std::string port)
{
	if (port.find_first_not_of("0123456789") != std::string::npos || port.length() > 5)
	{
		std::cerr <<"Error!" << std::endl;
		std::cerr <<"Ports go from 1025 to 65535 and accepts only digits! Ports below 1024 are reserved" <<std::endl;
		return (false);
	}

	int	portNb = atoi(port.c_str());
	if (portNb < 1024 || portNb > 65535)
	{
		std::cerr <<"Ports go from 1025 to 65535 and accepts only digits! Ports below 1024 are reserved" <<std::endl;
		return (false);
	}
	return (true);
}

// Password rules:
// Must be between 6 and 32 characters
// Must contain only alphanumeric characters
// PB: I did not found anything on the IRC documentation about this, so I guess this is up to us to decide

bool isValidPassword(const std::string& password)
{
	if (password.length() < 6 || password.length() > 32)
		return (false);

	for (size_t i = 0; i < password.length(); ++i)
	{
		if (!isalnum(password[i]))
		 return (false);
	}
	return true;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Incorrect number of arguments. Please write: ircserv <port> <password>" << std::endl;
		return (-1);
	}

	std::string port = argv[1];
	checkPortNumber(port);

	std::string password = argv[2];
	isValidPassword(password);


	// Start IRC
}
