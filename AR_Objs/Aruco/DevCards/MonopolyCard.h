//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_MONOPOLYCARD_H
#define AR_SETTLERS_MONOPOLYCARD_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class MonopolyCard : public DevelopmentCard {
public:
    MonopolyCard(int markerID) : DevelopmentCard(markerID) {}


    static ArucoObject* create(int markerID) {
        return new MonopolyCard(markerID);
    }

    static void registerMonopolyCard() {
        for (int markerID = 114; markerID <= 115; ++markerID) {
            ARObjectFactory::registerFactory(markerID, MonopolyCard::create);
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
#endif //AR_SETTLERS_MONOPOLYCARD_H
