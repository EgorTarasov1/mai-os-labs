#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <string>
#include <map>
#include "Player.h"

class GameManager {
public:
    std::string processRequest(const std::string& request);

private:
    std::map<std::string, Player> players;
    std::map<std::string, std::pair<std::string, std::string>> games; // Игра: {игрок1, игрок2}

    std::string handleLogin(const std::string& username);
    std::string handleCreateGame(const std::string& username, const std::string& gameName);
    std::string handleJoinGame(const std::string& username, const std::string& gameName);
    std::string handleStats(const std::string& username);
    void startGame(const std::string& gameName);
};

#endif