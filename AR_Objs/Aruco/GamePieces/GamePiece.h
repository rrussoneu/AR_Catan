//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_GAMEPIECE_H
#define AR_SETTLERS_GAMEPIECE_H
#pragma once
#include "../ArucoObject.h"

class GamePiece : public ArucoObject {
public:
    GamePiece(int markerID, RenderStrategy *renderStrategy = nullptr) : ArucoObject(markerID, renderStrategy) {}

    // Accessors for attributes used in rendering
    virtual const std::vector<std::vector<cv::Point3f>>& getPolygons() const = 0;

};

#endif //AR_SETTLERS_GAMEPIECE_H
