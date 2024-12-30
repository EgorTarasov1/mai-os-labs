#include "NodeManager.h"
#include <iostream>
#include <sstream>

int main() {
    NodeManager manager;
    std::string input;

    std::cout << "Distributed System Controller\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "create") {
            int id, parent = -1;
            iss >> id >> parent;
            std::cout << manager.createNode(id, parent) << std::endl;
        } 
        else if (command == "exec") {
            int id;
            std::string subcommand;
            iss >> id >> subcommand;
            std::cout << manager.execCommand(id, subcommand) << std::endl;
        } 
        else if (command == "ping") {
            int id;
            iss >> id;
            std::cout << manager.pingNode(id) << std::endl;
        } 
        else if (command == "kill") {
            int id;
            iss >> id;
            std::cout << manager.killNode(id) << std::endl;
        } 
        else if (command == "exit") {
            break;
        } 
        else {
            std::cout << "Error: Unknown command" << std::endl;
        }
    }

    return 0;
}