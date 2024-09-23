//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_OREHEX_H
#define AR_SETTLERS_OREHEX_H
#pragma once
#include "HexTile.h"
#include "../../ARObjectFactory.h"

class OreHex : public HexTile {
public:
    OreHex(int markerID) : HexTile(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new OreHex(markerID);
    }

    static void registerOreHex() {
        for (int markerID = 137; markerID <= 139; ++markerID) {
            ARObjectFactory::registerFactory(markerID, OreHex::create);
        }

    }

};
#endif //AR_SETTLERS_OREHEX_H
