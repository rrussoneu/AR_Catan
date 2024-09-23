//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WOOLHEX_H
#define AR_SETTLERS_WOOLHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class WoolHex : public HexTile {
public:
    WoolHex(int markerID) : HexTile(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new WoolHex(markerID);
    }

    static void registerWoolHex() {
        for (int markerID = 144; markerID <= 147; ++markerID) {
            ARObjectFactory::registerFactory(markerID, WoolHex::create);
        }

    }

};
#endif //AR_SETTLERS_WOOLHEX_H
