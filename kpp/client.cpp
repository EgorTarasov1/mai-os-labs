#include <iostream>
#include <windows.h>
#include <string>
#include "GameClient.h"

int main() {
    GameClient client;

    std::string username;
    std::cout << "Enter your username: ";
    std::cin >> username;

    client.sendRequest("LOGIN " + username);

    while (true) {
        std::cout << "Choose action: 1) Create Game 2) Join Game 3) View Stats: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::string gameName;
            std::cout << "Enter game name: ";
            std::cin >> gameName;
            std::string response = client.sendRequest("CREATE " + username + " " + gameName);
            std::cout << response << std::endl;
        } else if (choice == 2) {
            std::string gameName;
            std::cout << "Enter game name: ";
            std::cin >> gameName;
            std::string response = client.sendRequest("JOIN " + username + " " + gameName);
            std::cout << response << std::endl;
        } else if (choice == 3) {
            std::string response = client.sendRequest("STATS " + username);
            std::cout << response << std::endl;
        } else {
            std::cout << "Invalid choice!" << std::endl;
        }
    }

    return 0;
}