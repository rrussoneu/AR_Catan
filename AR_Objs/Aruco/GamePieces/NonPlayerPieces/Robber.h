//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ROBBER_H
#define AR_SETTLERS_ROBBER_H
#pragma once
#include "NonPlayerPiece.h"
#include "../../../ARObjectFactory.h"

class Robber : public NonPlayerPiece {
public:
    Robber(int markerID) : NonPlayerPiece(markerID) {}

    static ARObject* create(int markerID) {
        if (markerID == 97) return new Robber(markerID);
        return nullptr;
    }

    static void registerRobber() {
        ARObjectFactory::registerFactory(97, Robber::create);
    }

    void render(cv::Mat &frame) {}
};

#endif //AR_SETTLERS_ROBBER_H
