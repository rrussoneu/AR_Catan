//
// Created by Raphael Russo on 9/22/24.
// A class representing a resource card
//

#ifndef AR_SETTLERS_RESOURCECARD_H
#define AR_SETTLERS_RESOURCECARD_H
#pragma once
#include "../ArucoObject.h"

class ResourceCard : public ArucoObject {
public:
    ResourceCard(int markerID, RenderStrategy *renderStrategy = nullptr, const std::string &objectName = "") : ArucoObject(markerID, renderStrategy, objectName) {}
};

#endif //AR_SETTLERS_RESOURCECARD_H
