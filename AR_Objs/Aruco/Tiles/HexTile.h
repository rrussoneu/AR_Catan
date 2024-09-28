//
// Created by Raphael Russo on 9/22/24.
// A class representing a game hexagon / tile
//

#ifndef AR_SETTLERS_HEXTILE_H
#define AR_SETTLERS_HEXTILE_H
#pragma once
#include "../ArucoObject.h"

class HexTile : public ArucoObject {
public:
    HexTile(int markerID, RenderStrategy *renderStrategy = nullptr, const std::string &objectName = "") : ArucoObject(markerID, renderStrategy, objectName) {}
};
#endif //AR_SETTLERS_HEXTILE_H
