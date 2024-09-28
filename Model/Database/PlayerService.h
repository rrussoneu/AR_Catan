//
// Created by Raphael Russo on 9/28/24.
//

#ifndef AR_SETTLERS_PLAYERSERVICE_H
#define AR_SETTLERS_PLAYERSERVICE_H
#pragma once
#include "../Player.h"

class PlayerService {
public:
    bool fetchOrCreatePlayer(Player &player);
};
#endif //AR_SETTLERS_PLAYERSERVICE_H
