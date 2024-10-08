//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_NONPLAYERPIECE_H
#define AR_SETTLERS_NONPLAYERPIECE_H
#pragma once
#include "../GamePiece.h"

class NonPlayerPiece : public GamePiece {
public:
    NonPlayerPiece(int markerID, RenderStrategy *renderStrategy = nullptr) : GamePiece(markerID, renderStrategy) {}
};
#endif //AR_SETTLERS_NONPLAYERPIECE_H
