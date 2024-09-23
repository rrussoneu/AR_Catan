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

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new WheatResCard(markerID);
    }

    static void registerWheatResCard() {
        ARObjectFactory::registerFactory(128, WheatResCard::create);

    }

};
#endif //AR_SETTLERS_WHEATRESCARD_H
