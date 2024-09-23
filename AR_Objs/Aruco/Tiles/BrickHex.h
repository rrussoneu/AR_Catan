//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_BRICKHEX_H
#define AR_SETTLERS_BRICKHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class BrickHex : public HexTile {
public:
    BrickHex(int markerID) : HexTile(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new BrickHex(markerID);
    }

    static void registerBrickHex() {
        for (int markerID = 134; markerID <= 136; ++markerID) {
            ARObjectFactory::registerFactory(markerID, BrickHex::create);
        }

    }

};
#endif //AR_SETTLERS_BRICKHEX_H
