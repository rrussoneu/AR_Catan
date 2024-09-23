//
// Created by Raphael Russo on 9/22/24.
// A class representing a development card
//

#ifndef AR_SETTLERS_DEVELOPMENTCARD_H
#define AR_SETTLERS_DEVELOPMENTCARD_H

#pragma once
#include "../ArucoObject.h"

// Add later: public HomographyObj
class DevelopmentCard : public ArucoObject {
public:
    DevelopmentCard(int markerID) : ArucoObject(markerID) {}
};

#endif //AR_SETTLERS_DEVELOPMENTCARD_H
