#include "Player.h"

Player::Player(const std::string& username) : username(username), wins(0), losses(0) {}

int Player::getWins() const {
    return wins;
}

int Player::getLosses() const {
    return losses;
}

void Player::addWin() {
    wins++;
}

void Player::addLoss() {
    losses++;
}