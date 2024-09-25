//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ORERESCARD_H
#define AR_SETTLERS_ORERESCARD_H
#pragma once
#include "ResourceCard.h"
#include "../../ARObjectFactory.h"

class OreResCard : public ResourceCard {
public:
    OreResCard(int markerID) : ResourceCard(markerID) {}



    static ArucoObject* create(int markerID) {
        return new OreResCard(markerID);
    }

    static void registerOreResCard() {
        ARObjectFactory::registerFactory(127, OreResCard::create);

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
#endif //AR_SETTLERS_ORERESCARD_H
