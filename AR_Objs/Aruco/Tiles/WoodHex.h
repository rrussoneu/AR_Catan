//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WOODHEX_H
#define AR_SETTLERS_WOODHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class WoodHex : public HexTile {
public:
    WoodHex(int markerID) : HexTile(markerID) {}


    static ArucoObject* create(int markerID) {
        return new WoodHex(markerID);
    }

    static void registerWoodHex() {
        for (int markerID = 130; markerID <= 133; ++markerID) {
            ARObjectFactory::registerFactory(markerID, WoodHex::create);
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
#endif //AR_SETTLERS_WOODHEX_H
