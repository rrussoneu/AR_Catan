//
// Created by Raphael Russo on 9/28/24.
//

#include "PlayerService.h"
#include "DatabaseManager.h"

bool PlayerService::fetchOrCreatePlayer(Player& player) {
    bool exists = DatabaseManager::getInstance().getPlayerStats(player);
    if (!exists) {
        if (!DatabaseManager::getInstance().addNewPlayer(player)) {
            //qWarning("Failed to add new player to the database"); -- now in the update username command
            return false;
        }
    }
    return true;
}