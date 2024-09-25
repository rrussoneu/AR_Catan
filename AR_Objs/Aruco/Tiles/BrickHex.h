//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_BRICKHEX_H
#define AR_SETTLERS_BRICKHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class BrickHex : public HexTile {
public:
    BrickHex(int markerID) : HexTile(markerID) {}



    static ArucoObject* create(int markerID) {
        return new BrickHex(markerID);
    }

    static void registerBrickHex() {
        for (int markerID = 134; markerID <= 136; ++markerID) {
            ARObjectFactory::registerFactory(markerID, BrickHex::create);
        }

    }

    const std::vector<std::vector<cv::Point3f>>& getPolygons() const override {
        // Return empty or default value
        static std::vector<std::vector<cv::Point3f>> emptyPolygons;
        return emptyPolygons;
    }

    const std::vector<cv::Scalar>& getColors() const override {
        // Return empty or default value
        static std::vector<cv::Scalar> emptyColors;
        return emptyColors;
    }

};
#endif //AR_SETTLERS_BRICKHEX_H
