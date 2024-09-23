//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_YEAROFPLENTYCARD_H
#define AR_SETTLERS_YEAROFPLENTYCARD_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class YearOfPlentyCard : public DevelopmentCard {
public:
    YearOfPlentyCard(int markerID) : DevelopmentCard(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new YearOfPlentyCard(markerID);
    }

    static void registerYearOfPlentyCard() {
        for (int markerID = 116; markerID <= 117; ++markerID) {
            ARObjectFactory::registerFactory(markerID, YearOfPlentyCard::create);
        }
    }

};
#endif //AR_SETTLERS_YEAROFPLENTYCARD_H
