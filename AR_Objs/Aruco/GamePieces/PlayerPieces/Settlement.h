//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_SETTLEMENT_H
#define AR_SETTLERS_SETTLEMENT_H
#pragma once
#include "PlayerPiece.h"
#include "../../../ARObjectFactory.h"


class Settlement : public PlayerPiece {
public:
    Settlement(int markerID, const std::string &color) : PlayerPiece(markerID, color) {
        std::vector<std::vector<cv::Point2f>> polygons;
    }

    static ArucoObject* create(int markerID) {
        if (markerID >= 61 && markerID <= 65) return new Settlement(markerID, "blue");
        if (markerID >= 66 && markerID <= 70) return new Settlement(markerID, "red");
        if (markerID >= 71 && markerID <= 75) return new Settlement(markerID, "orange");
        if (markerID >= 76 && markerID <= 80) return new Settlement(markerID, "white");
        return nullptr;
    }

    static void registerSettlement() {
        for (int markerID = 61; markerID <= 80; ++markerID) {
            ARObjectFactory::registerFactory(markerID, Settlement::create);
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
private:
    std::vector<std::vector<cv::Point2f>> polygons;
    //std::vector<

};
#endif //AR_SETTLERS_SETTLEMENT_H
