//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_KNIGHTCARD_H
#define AR_SETTLERS_KNIGHTCARD_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class KnightCard : public DevelopmentCard {
public:
    KnightCard(int markerID) : DevelopmentCard(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new KnightCard(markerID);
    }

    static void registerKnightCard() {
        for (int markerID = 100; markerID <= 113; ++markerID) {
            ARObjectFactory::registerFactory(markerID, KnightCard::create);
        }
    }

};
#endif //AR_SETTLERS_KNIGHTCARD_H
