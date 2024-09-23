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
    ResourceCard(int markerID) : ArucoObject(markerID) {}
};

#endif //AR_SETTLERS_RESOURCECARD_H
