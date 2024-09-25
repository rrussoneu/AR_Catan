//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_YEAROFPLENTYCARD_H
#define AR_SETTLERS_YEAROFPLENTYCARD_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class YearOfPlentyCard : public DevelopmentCard {
public:
    YearOfPlentyCard(int markerID) : DevelopmentCard(markerID) {}



    static ArucoObject* create(int markerID) {
        return new YearOfPlentyCard(markerID);
    }

    static void registerYearOfPlentyCard() {
        for (int markerID = 116; markerID <= 117; ++markerID) {
            ARObjectFactory::registerFactory(markerID, YearOfPlentyCard::create);
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
#endif //AR_SETTLERS_YEAROFPLENTYCARD_H
