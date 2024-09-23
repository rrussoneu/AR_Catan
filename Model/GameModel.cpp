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