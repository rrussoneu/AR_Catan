//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WOOLRESCARD_H
#define AR_SETTLERS_WOOLRESCARD_H
#pragma once
#include "ResourceCard.h"
#include "../../ARObjectFactory.h"

class WoolResCard : public ResourceCard {
public:
    WoolResCard(int markerID) : ResourceCard(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new WoolResCard(markerID);
    }

    static void registerWoolResCard() {
        ARObjectFactory::registerFactory(129, WoolResCard::create);

    }

};
#endif //AR_SETTLERS_WOOLRESCARD_H
