//
// Created by Raphael Russo on 9/24/24.
//

#ifndef AR_SETTLERS_RENDERSTRATEGY_H
#define AR_SETTLERS_RENDERSTRATEGY_H
#pragma once

#include "../Aruco/ArucoObject.h"
#include <opencv2/opencv.hpp>

class ArucoObject;

class RenderStrategy {
public:
    virtual ~RenderStrategy() = default;
    virtual void render(ArucoObject *object, cv::Mat &frame, const std::vector<cv::Point2f> &markerCorners, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const std::vector<double> &distCoeffs, const cv::Mat &cameraMatrix) = 0;
};
#endif //AR_SETTLERS_RENDERSTRATEGY_H
