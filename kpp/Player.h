#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    Player() : username(""), wins(0), losses(0) {} // Конструктор по умолчанию
    Player(const std::string& username);
    int getWins() const;
    int getLosses() const;
    void addWin();
    void addLoss();

private:
    std::string username;
    int wins;
    int losses;
};

#endif