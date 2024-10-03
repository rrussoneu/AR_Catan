//
// Created by Raphael Russo on 9/28/24.
//

#ifndef AR_SETTLERS_DATABASEMANAGER_H
#define AR_SETTLERS_DATABASEMANAGER_H
#pragma once


#include <QObject>
#include <QSqlDatabase>
#include <QMutex>
#include "../Player.h"
#include "../MessageEmitter.h"

class DatabaseManager : public MessageEmitter{

public:
    static DatabaseManager& getInstance();
    ~DatabaseManager();

    // Connection
    bool openDatabase(const QString &path);
    void closeDatabase();

    // Repository
    bool getPlayerStats(Player &player);
    bool updatePlayerStats(const Player &player);
    bool addNewPlayer(const Player &player);

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase db;
    static QMutex mutex;

};


#endif //AR_SETTLERS_DATABASEMANAGER_H
