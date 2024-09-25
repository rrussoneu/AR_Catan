//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ROBBER_H
#define AR_SETTLERS_ROBBER_H
#pragma once
#include "NonPlayerPiece.h"
#include "../../../ARObjectFactory.h"

class Robber : public NonPlayerPiece {
public:
    Robber(int markerID) : NonPlayerPiece(markerID) {}

    static ArucoObject* create(int markerID) {
        if (markerID == 97) return new Robber(markerID);
        return nullptr;
    }

    static void registerRobber() {
        ARObjectFactory::registerFactory(97, Robber::create);
    }

    void render(cv::Mat &frame, const std::vector<cv::Point2f> &markerCorners, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const std::vector<double> &distCoeffs, const cv::Mat &cameraMatrix) override {
        // Add logic
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

#endif //AR_SETTLERS_ROBBER_H
