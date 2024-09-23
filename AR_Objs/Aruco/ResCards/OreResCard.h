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

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new OreResCard(markerID);
    }

    static void registerOreResCard() {
        ARObjectFactory::registerFactory(127, OreResCard::create);

    }

};
#endif //AR_SETTLERS_ORERESCARD_H
