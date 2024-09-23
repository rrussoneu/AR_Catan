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
    WoodResCard(int markerID) : ResourceCard(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new WoodResCard(markerID);
    }

    static void registerWoodResCard() {
        ARObjectFactory::registerFactory(125, WoodResCard::create);

    }

};
#endif //AR_SETTLERS_WOODRESCARD_H
