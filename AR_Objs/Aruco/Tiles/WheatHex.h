//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WHEATHEX_H
#define AR_SETTLERS_WHEATHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class WheatHex : public HexTile {
public:
    WheatHex(int markerID, RenderStrategy *renderStrategy = nullptr, const std::string &objectName = "wheat") : HexTile(markerID, renderStrategy, objectName) {}



    static ArucoObject* create(int markerID) {
        return new WheatHex(markerID);
    }

    static void registerWheatHex() {
        for (int markerID = 140; markerID <= 143; ++markerID) {
            ARObjectFactory::registerFactory(markerID, WheatHex::create);
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
#endif //AR_SETTLERS_WHEATHEX_H
