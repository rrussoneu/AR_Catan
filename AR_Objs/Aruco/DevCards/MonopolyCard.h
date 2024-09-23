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

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new MonopolyCard(markerID);
    }

    static void registerMonopolyCard() {
        for (int markerID = 114; markerID <= 115; ++markerID) {
            ARObjectFactory::registerFactory(markerID, MonopolyCard::create);
        }
    }

};
#endif //AR_SETTLERS_MONOPOLYCARD_H
