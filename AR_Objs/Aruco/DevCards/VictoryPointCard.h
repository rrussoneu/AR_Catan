//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_VICTORYPOINTCARD_H
#define AR_SETTLERS_VICTORYPOINTCARD_H
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class VictoryPointCard : public DevelopmentCard {
public:
    VictoryPointCard(int markerID) : DevelopmentCard(markerID) {}

    void render(cv::Mat& frame) override {

    }

    static ARObject* create(int markerID) {
        return new VictoryPointCard(markerID);
    }

    static void registerVictoryPointCard() {
        for (int markerID = 120; markerID <= 124; ++markerID) {
            ARObjectFactory::registerFactory(markerID, VictoryPointCard::create);
        }
    }

};
#endif //AR_SETTLERS_VICTORYPOINTCARD_H
