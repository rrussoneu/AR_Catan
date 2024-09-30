//
// Created by Raphael Russo on 9/23/24.
//

#include <QMessageBox>
#include "GameModel.h"
#include "Database/DatabaseManager.h"


GameModel::GameModel(QObject *parent) : QObject(parent) {}

void GameModel::addPlayer(const QString &color, const Player &player) {
    players.insert(color, player);
}

Player* GameModel::getPlayer(const QString &color) {
    if (players.contains(color)) {
        return &players[color];
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

QList<Player*> GameModel::getAllPlayers() {
    QList<Player*> playerList;
    for (auto it = players.begin(); it != players.end(); ++it) {
        playerList.append(&it.value());
    }
    return playerList;
}

void GameModel::finishGame() {
    // Retrieve all players from the model
    QList<Player*> allPlayers = getAllPlayers();

    if (allPlayers.isEmpty()) {
        emit errorOccurred("There are no players to process.");
        //QMessageBox::warning(nullptr, "No Players", "There are no players to process.");
        return;
    }

    // Determine the winner(s)
    int highestScore = INT_MIN;
    QList<Player*> winners;

    for (Player* player : allPlayers) {
        if (player->getScore() > highestScore) {
            highestScore = player->getScore();
            winners.clear();
            winners.append(player);
        } else if (player->getScore() == highestScore) {
            winners.append(player); // Handle tie in event of game ending early or something
        }
    }

    // Update stats for all players
    for (Player* player : allPlayers) {

        //DatabaseManager::getInstance().getPlayerStats(*player);
        if (player->getUsername() == "guest") {
            emit errorOccurred("Guest skipped in stat updates");
            //qWarning("Guest player being skipped.");
            continue;
        }

        // Increment games played
        player->setGamesPlayed(player->getGamesPlayed() + 1);

        // Update total score (add current game score)
        player->setTotalScore(player->getTotalScore() + player->getScore());

        // Check if player is a winner
        if (winners.contains(player)) {
            player->setWins(player->getWins() + 1);
        }

        // Save updated stats to the database
        if (!DatabaseManager::getInstance().updatePlayerStats(*player)) {
            //QMessageBox::warning(nullptr, "Database Error", "Failed to update stats for player: " + player->getUsername());
            emit errorOccurred("Database error - failed to update player stats for: " + player->getUsername());
        }
    }

    // Display a message with the winner(s)
    QString winnerNames;
    for (Player* winner : winners) {
        winnerNames += winner->getUsername() + " ";
    }
    emit sendMessage("Game finished. Winner(s): " + winnerNames);
    //QMessageBox::information(nullptr, "Game Finished", "The game has ended.\nWinner(s): " + winnerNames);
}