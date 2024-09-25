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



    static ArucoObject* create(int markerID) {
        return new VictoryPointCard(markerID);
    }

    static void registerVictoryPointCard() {
        for (int markerID = 120; markerID <= 124; ++markerID) {
            ARObjectFactory::registerFactory(markerID, VictoryPointCard::create);
        }
    }

    const std::vector<std::vector<cv::Point3f>>& getPolygons() const override {
        // Return empty or default value
        static std::vector<std::vector<cv::Point3f>> emptyPolygons;
        return emptyPolygons;
    }

    const std::vector<cv::Scalar>& getColors() const override {
        // Return empty or default value
        static std::vector<cv::Scalar> emptyColors;
        return emptyColors;
    }

};
#endif //AR_SETTLERS_VICTORYPOINTCARD_H
