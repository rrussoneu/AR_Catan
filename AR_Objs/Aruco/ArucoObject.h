//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ARUCOOBJECT_H
#define AR_SETTLERS_ARUCOOBJECT_H
#pragma once
#include "../ARObject.h"
#include "../Rendering/RenderStrategy.h"
#include <opencv2/opencv.hpp>
#include <vector>

class ArucoObject : public ARObject {
public:
    ArucoObject(int markerID, RenderStrategy* strategy = nullptr)
            : markerID(markerID), renderStrategy(strategy) {}

    virtual ~ArucoObject() = default;

    int getMarkerID() const { return markerID; }

    void setRenderStrategy(RenderStrategy* strategy) {
        renderStrategy = strategy;
    }

    void render(cv::Mat &frame, const std::vector<cv::Point2f> &markerCorners, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const std::vector<double> &distCoeffs, const cv::Mat &cameraMatrix) override {
        if (renderStrategy) {
            renderStrategy->render(this, frame, markerCorners, rvec, tvec, distCoeffs, cameraMatrix);
        }
    }

    virtual const std::vector<std::vector<cv::Point3f>>& getPolygons() const = 0;
    virtual const std::vector<cv::Scalar>& getColors() const = 0;

protected:
    int markerID;
    RenderStrategy* renderStrategy;
};
#endif //AR_SETTLERS_ARUCOOBJECT_H
