//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_PLAYERPIECE_H
#define AR_SETTLERS_PLAYERPIECE_H
#pragma once
#include "../GamePiece.h"

class PlayerPiece : public GamePiece {
public:
    PlayerPiece(int markerID, const std::string& color) : GamePiece(markerID), color(color) {}

    std::string getColor() const {
        return color;
    }

protected:
    std::string color; // Red, blue, orange, white
};
#endif //AR_SETTLERS_PLAYERPIECE_H
