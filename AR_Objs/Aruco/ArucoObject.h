//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ARUCOOBJECT_H
#define AR_SETTLERS_ARUCOOBJECT_H
#pragma once
#include "../ARObject.h"

class ArucoObject : public ARObject {
public:
    ArucoObject(int markerID) : markerID(markerID) {}

    int getMarkerID() const {
        return markerID;
    }
protected:
    int markerID;
};
#endif //AR_SETTLERS_ARUCOOBJECT_H
