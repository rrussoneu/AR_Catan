//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_CITY_H
#define AR_SETTLERS_CITY_H
#pragma once
#include "PlayerPiece.h"
#include "../../../ARObjectFactory.h"

class City : public PlayerPiece {
public:
    City(int markerID, const std::string &color) : PlayerPiece(markerID, color) {}

    static ARObject* create(int markerID) {
        if (markerID >= 81 && markerID <= 84) return new City(markerID, "blue");
        if (markerID >= 85 && markerID <= 88) return new City(markerID, "red");
        if (markerID >= 89 && markerID <= 92) return new City(markerID, "orange");
        if (markerID >= 93 && markerID <= 96) return new City(markerID, "white");
        return nullptr;
    }

    static void registerCity() {
        for (int markerID = 81; markerID <= 96; ++markerID) {
            ARObjectFactory::registerFactory(markerID, City::create);
        }
    }

    void render(cv::Mat &frame) {}
};
#endif //AR_SETTLERS_CITY_H
