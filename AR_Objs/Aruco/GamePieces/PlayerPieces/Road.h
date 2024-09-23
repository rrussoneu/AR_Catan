//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ROAD_H
#define AR_SETTLERS_ROAD_H
#pragma once
#include "PlayerPiece.h"
#include "../../../ARObjectFactory.h"

class Road : public PlayerPiece {
public:
    Road(int markerID, const std::string &color) : PlayerPiece(markerID, color) {}

    static ARObject* create(int markerID) {
        if (markerID >= 1 && markerID <= 15) return new Road(markerID, "blue");
        if (markerID >= 16 && markerID <= 30) return new Road(markerID, "red");
        if (markerID >= 31 && markerID <= 45) return new Road(markerID, "orange");
        if (markerID >= 46 && markerID <= 60) return new Road(markerID, "white");
        return nullptr;
    }

    static void registerRoad() {
        for (int markerID = 1; markerID <= 60; ++markerID) {
            ARObjectFactory::registerFactory(markerID, Road::create);
        }
    }

    void render(cv::Mat &frame) {}
};
#endif //AR_SETTLERS_ROAD_H
