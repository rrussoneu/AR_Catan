//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_GAMEPIECE_H
#define AR_SETTLERS_GAMEPIECE_H
#pragma once
#include "../ArucoObject.h"

class GamePiece : public ArucoObject {
public:
    GamePiece(int markerID) : ArucoObject(markerID) {}

};

#endif //AR_SETTLERS_GAMEPIECE_H
