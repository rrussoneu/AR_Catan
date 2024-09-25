//
// Created by Raphael Russo on 9/23/24.
//

#include "GameModel.h"


void GameModel::addPlayer(const Player &player) {
    players.emplace(player.getColor(), player);
}

Player* GameModel::getPlayer(const std::string &color) {
    auto it = players.find(color);
    if (it != players.end()) {
        return &it->second;
    }
    return nullptr;
}

int GameModel::rollDice() {
    // Seed random number generator
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
    // 2 to 12
    return (std::rand() % 11) + 2;
}