//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_BRICKRESCARD_H
#define AR_SETTLERS_BRICKRESCARD_H
#pragma once
#include "ResourceCard.h"
#include "../../ARObjectFactory.h"

class BrickResCard : public ResourceCard {
public:
    BrickResCard(int markerID, RenderStrategy *renderStrategy = nullptr, const std::string &objectName = "brick") : ResourceCard(markerID, renderStrategy, objectName) {}


    static ArucoObject* create(int markerID) {
        return new BrickResCard(markerID);
    }

    static void registerBrickResCard() {
        ARObjectFactory::registerFactory(126, BrickResCard::create);

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
#endif //AR_SETTLERS_BRICKRESCARD_H
