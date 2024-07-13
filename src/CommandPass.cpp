#include "CommandPass.hpp"
#include <iostream>
#include <vector>
#include <string>

CommandPass::CommandPass() {}
CommandPass::~CommandPass(){}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type end;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
    }

    // Add the last substring
    result.push_back(str.substr(start));

    return result;
}

void CommandPass::execute(int &clientSocket, Client *client, Server *server, std::string payload) {
	std::cout << "COMMAND: PASS - payload:" << payload << std::endl;
	std::vector<std::string> strings = split(payload, ' ');
	if(strings.size() > 1){
		server->sendResponse(clientSocket,"TO BE IMPLEMENTED!!! - return error");
	}
	//client->pendingWrite.push_back("TO BE IMPLEMENTED!!!");
}