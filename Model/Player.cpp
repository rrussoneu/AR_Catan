//
// Created by Raphael Russo on 9/23/24.
//

#include "Player.h"

Player::Player(const std::string &color) : color(color), username("guest"), score(0), wins(0), averageScore(0.0), gamesPlayed(0) {}

std::string Player::getUsername() const {
    return username;
}

void Player::setUsername(const std::string& username) {
    this->username = username;
}

int Player::getWins() const {
    return wins;
}

void Player::setWins(int wins) {
    this->wins = wins;
}

int Player::getGamesPlayed() const {
    return gamesPlayed;
}

void Player::setGamesPlayed(int gamesPlayed) {
    this->gamesPlayed = gamesPlayed;
}

int Player::getScore() const {
    return score;
}

void Player::setScore(int score) {
    this->score = score;
}

void Player::incrementScore() {
    ++score;
}

void Player::decrementScore() {
    if (score > 0) {
        --score;
    }
}

float Player::getAverageScore() const {
    // Once database added this can be implemented
    return averageScore;
}

std::string Player::getColor() const {
    return color;
}
