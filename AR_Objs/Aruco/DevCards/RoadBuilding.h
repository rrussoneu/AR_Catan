//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ROADBUILDING_H
#define AR_SETTLERS_ROADBUILDING_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class RoadBuildingCard : public DevelopmentCard {
public:
    RoadBuildingCard(int markerID) : DevelopmentCard(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new RoadBuildingCard(markerID);
    }

    static void registerRoadBuildingCard() {
        for (int markerID = 118; markerID <= 119; ++markerID) {
            ARObjectFactory::registerFactory(markerID, RoadBuildingCard::create);
        }
    }

};
#endif //AR_SETTLERS_ROADBUILDING_H
