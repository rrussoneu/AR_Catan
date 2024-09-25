//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WOOLHEX_H
#define AR_SETTLERS_WOOLHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class WoolHex : public HexTile {
public:
    WoolHex(int markerID) : HexTile(markerID) {}


    static ArucoObject* create(int markerID) {
        return new WoolHex(markerID);
    }

    static void registerWoolHex() {
        for (int markerID = 144; markerID <= 147; ++markerID) {
            ARObjectFactory::registerFactory(markerID, WoolHex::create);
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
#endif //AR_SETTLERS_WOOLHEX_H
