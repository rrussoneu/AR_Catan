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
    BrickResCard(int markerID) : ResourceCard(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new BrickResCard(markerID);
    }

    static void registerBrickResCard() {
        ARObjectFactory::registerFactory(126, BrickResCard::create);

    }

};
#endif //AR_SETTLERS_BRICKRESCARD_H
