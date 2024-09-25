//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_KNIGHTCARD_H
#define AR_SETTLERS_KNIGHTCARD_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"


class KnightCard : public DevelopmentCard {
public:
    KnightCard(int markerID)
            : DevelopmentCard(markerID) {}

    void render(cv::Mat &frame, const std::vector<cv::Point2f> &markerCorners, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const std::vector<double> &distCoeffs, const cv::Mat &cameraMatrix) override {
        // Add logic
    }

    static void registerKnightCard() {
        for (int markerID = 100; markerID <= 113; ++markerID) {
            ARObjectFactory::registerFactory(markerID, KnightCard::create);
        }
    }

    static ArucoObject* create(int markerID) {
        return new KnightCard(markerID);
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


#endif //AR_SETTLERS_KNIGHTCARD_H
