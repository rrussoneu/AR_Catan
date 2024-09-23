//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WOODHEX_H
#define AR_SETTLERS_WOODHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class WoodHex : public HexTile {
public:
    WoodHex(int markerID) : HexTile(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new WoodHex(markerID);
    }

    static void registerWoodHex() {
        for (int markerID = 130; markerID <= 133; ++markerID) {
            ARObjectFactory::registerFactory(markerID, WoodHex::create);
        }

    }

};
#endif //AR_SETTLERS_WOODHEX_H
