//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WOODRESCARD_H
#define AR_SETTLERS_WOODRESCARD_H
#pragma once
#include "ResourceCard.h"
#include "../../ARObjectFactory.h"

class WoodResCard : public ResourceCard {
public:
    WoodResCard(int markerID, RenderStrategy *renderStrategy = nullptr, const std::string &objectName = "wood") : ResourceCard(markerID, renderStrategy, objectName) {}



    static ArucoObject* create(int markerID) {
        return new WoodResCard(markerID);
    }

    static void registerWoodResCard() {
        ARObjectFactory::registerFactory(125, WoodResCard::create);

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
#endif //AR_SETTLERS_WOODRESCARD_H
