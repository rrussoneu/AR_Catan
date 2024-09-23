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
    HexTile(int markerID) : ArucoObject(markerID) {}
};
#endif //AR_SETTLERS_HEXTILE_H
