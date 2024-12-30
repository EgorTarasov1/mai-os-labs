#include "GameManager.h"
#include <iostream> // Добавлено
#include <thread>

std::string GameManager::processRequest(const std::string& request) {
    if (request.find("LOGIN") == 0) {
        std::string username = request.substr(6);
        return handleLogin(username);
    } else if (request.find("CREATE") == 0) {
        size_t pos = request.find(' ', 7);
        std::string username = request.substr(7, pos - 7);
        std::string gameName = request.substr(pos + 1);
        return handleCreateGame(username, gameName);
    } else if (request.find("JOIN") == 0) {
        size_t pos = request.find(' ', 5);
        std::string username = request.substr(5, pos - 5);
        std::string gameName = request.substr(pos + 1);
        return handleJoinGame(username, gameName);
    } else if (request.find("STATS") == 0) {
        std::string username = request.substr(6);
        return handleStats(username);
    } else {
        return "INVALID_REQUEST";
    }
}

std::string GameManager::handleLogin(const std::string& username) {
    players[username] = Player(username);
    return "LOGIN_SUCCESS";
}

std::string GameManager::handleCreateGame(const std::string& username, const std::string& gameName) {
    games[gameName] = {username, ""};
    return "GAME_CREATED:" + gameName;
}

std::string GameManager::handleJoinGame(const std::string& username, const std::string& gameName) {
    if (games.find(gameName) != games.end() && games[gameName].second.empty()) {
        games[gameName].second = username;
        startGame(gameName);
        return "JOIN_SUCCESS:" + games[gameName].first;
    } else {
        return "GAME_NOT_FOUND";
    }
}

std::string GameManager::handleStats(const std::string& username) {
    if (players.find(username) != players.end()) {
        return "STATS:" + std::to_string(players[username].getWins()) + ":" + std::to_string(players[username].getLosses());
    } else {
        return "USER_NOT_FOUND";
    }
}

void GameManager::startGame(const std::string& gameName) {
    std::string player1 = games[gameName].first;
    std::string player2 = games[gameName].second;

    // Отправляем сообщение о начале игры
    std::cout << "Game " << gameName << " started between " << player1 << " and " << player2 << std::endl;
}