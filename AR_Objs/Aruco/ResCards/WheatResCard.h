//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WHEATRESCARD_H
#define AR_SETTLERS_WHEATRESCARD_H
#pragma once
#include "ResourceCard.h"
#include "../../ARObjectFactory.h"

class WheatResCard : public ResourceCard {
public:
    WheatResCard(int markerID) : ResourceCard(markerID) {}



    static ArucoObject* create(int markerID) {
        return new WheatResCard(markerID);
    }

    static void registerWheatResCard() {
        ARObjectFactory::registerFactory(128, WheatResCard::create);

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
#endif //AR_SETTLERS_WHEATRESCARD_H
