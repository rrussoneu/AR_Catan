//
// Created by Raphael Russo on 9/28/24.
//

#include <QSqlQuery>
#include "DatabaseManager.h"

QMutex DatabaseManager::mutex;

// Singleton - private constructor
DatabaseManager::DatabaseManager(QObject *parent) : MessageEmitter(parent){}



DatabaseManager::~DatabaseManager() {
    closeDatabase(); // Close database connection
}

DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::openDatabase(const QString &path) {
    QMutexLocker locker(&mutex);

    // Return true / do nothing if the db is open
    if(db.isOpen()) {
        return true;
    }

    // Otherwise connect to the database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    // Return false if issue
    if (!db.open()) {
        emit sendError("Failed to open db");
        return false;
    }

    QSqlQuery query;
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS Players (
            username TEXT PRIMARY KEY,
            wins INTEGER DEFAULT 0,
            gamesPlayed INTEGER DEFAULT 0,
            totalScore INTEGER DEFAULT 0
        )
    )";

    if (!query.exec(createTable)) {
        emit sendError("Failed to create table");
        return false;
    }

    return true;

}

void DatabaseManager::closeDatabase() {
    QMutexLocker locker(&mutex);

    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::getPlayerStats(Player &player) {
    QMutexLocker locker(&mutex);

    //Player player(username);

    QSqlQuery query;
    query.prepare("SELECT wins, gamesPlayed, totalScore FROM players WHERE username = :username");
    query.bindValue(":username", player.getUsername());

    if (query.exec() && query.next()) {
        player.setWins(query.value("wins").toInt());
        player.setGamesPlayed(query.value("gamesPlayed").toInt());
        player.setTotalScore(query.value("totalScore").toInt());
        return true; // Player exists
    } else {
        emit sendError("Player not found. Adding as new player");
        return false; // Player does not exist
    }


}

bool DatabaseManager::addNewPlayer(const Player &player) {
    QMutexLocker locker(&mutex);

    QSqlQuery query;
    query.prepare("INSERT INTO players (username, wins, gamesPlayed, totalScore) VALUES (:username, :wins, :gamesPlayed, :totalScore)");
    query.bindValue(":username", player.getUsername());
    query.bindValue(":wins", player.getWins());
    query.bindValue(":gamesPlayed", player.getGamesPlayed());
    query.bindValue(":totalScore", player.getTotalScore());

    if (!query.exec()) {
        emit sendError("Failed to add new player");
        return false;
    }

    return true;
}

bool DatabaseManager::updatePlayerStats(const Player &player) {
    QMutexLocker locker(&mutex);

    QSqlQuery query;
    query.prepare("UPDATE players SET wins = :wins, gamesPlayed = :gamesPlayed, totalScore = :totalScore WHERE username = :username");
    query.bindValue(":username", player.getUsername());
    query.bindValue(":wins", player.getWins());
    query.bindValue(":gamesPlayed", player.getGamesPlayed());
    query.bindValue(":totalScore", player.getTotalScore());
    if (!query.exec()) {
        emit sendError("Failed to update player stats");
        return false;
    }

    return true;
}
