#include "Server.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
	}
	std::string port = argv[1];
	// Need to check if port is a valid number
	std::string passwd = argv[2];
	// Start IRC
}
