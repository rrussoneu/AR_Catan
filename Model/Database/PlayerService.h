//
// Created by Raphael Russo on 9/28/24.
//

#ifndef AR_SETTLERS_PLAYERSERVICE_H
#define AR_SETTLERS_PLAYERSERVICE_H
#pragma once
#include "../Player.h"
#include "../MessageEmitter.h"

class PlayerService : public MessageEmitter {
public:
    explicit PlayerService(QObject* parent = nullptr): MessageEmitter(parent) {}
    bool fetchOrCreatePlayer(Player &player);
};
#endif //AR_SETTLERS_PLAYERSERVICE_H
