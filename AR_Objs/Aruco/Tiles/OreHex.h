//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_OREHEX_H
#define AR_SETTLERS_OREHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class OreHex : public HexTile {
public:
    OreHex(int markerID) : HexTile(markerID) {}



    static ArucoObject* create(int markerID) {
        return new OreHex(markerID);
    }

    static void registerOreHex() {
        for (int markerID = 137; markerID <= 139; ++markerID) {
            ARObjectFactory::registerFactory(markerID, OreHex::create);
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
#endif //AR_SETTLERS_OREHEX_H
