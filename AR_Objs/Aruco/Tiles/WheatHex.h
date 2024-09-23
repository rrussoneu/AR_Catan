//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_WHEATHEX_H
#define AR_SETTLERS_WHEATHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class WheatHex : public HexTile {
public:
    WheatHex(int markerID) : HexTile(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new WheatHex(markerID);
    }

    static void registerWheatHex() {
        for (int markerID = 140; markerID <= 143; ++markerID) {
            ARObjectFactory::registerFactory(markerID, WheatHex::create);
        }

    }

};
#endif //AR_SETTLERS_WHEATHEX_H
