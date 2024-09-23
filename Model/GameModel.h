//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_GAMEMODEL_H
#define AR_SETTLERS_GAMEMODEL_H
#pragma once
#include <unordered_map>
#include "../AR_Objs/ARObject.h"
#include "Player.h"

class GameModel {
public:
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

    void addPlayer(const Player &player);
    Player *getPlayer(const std::string &color);


private:
    std::unordered_map<int, ARObject*> objectMap;
    std::unordered_map<std::string, Player> players;
};
#endif //AR_SETTLERS_GAMEMODEL_H
