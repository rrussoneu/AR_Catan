//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_GAMEMODEL_H
#define AR_SETTLERS_GAMEMODEL_H
#pragma once
#include <unordered_map>
#include <QString>
#include "../AR_Objs/ARObject.h"
#include "Player.h"
#include "MessageEmitter.h"
#include <QMap>
#include <QObject>

class GameModel: public MessageEmitter {
public:
    explicit GameModel(QObject *parent = nullptr);
    ~GameModel() {
        // Delete all ARObjects
        for (const auto& pair : objectMap) {
            delete pair.second;
        }
    }
    void addObject(int markerID, ARObject* obj) {
        objectMap[markerID] = obj;
    }

    ARObject* getObject(int markerID) {
        auto it = objectMap.find(markerID);
        if (it != objectMap.end()) {
            return it->second;
        }
        return nullptr;
    }

    QList<Player*> getAllPlayers();

    void addPlayer(const QString &color, const Player &player);
    Player* getPlayer(const QString &color);

    int rollDice();

    void finishGame();


private:
    std::unordered_map<int, ARObject*> objectMap;
    QMap<QString, Player> players;
};
#endif //AR_SETTLERS_GAMEMODEL_H
