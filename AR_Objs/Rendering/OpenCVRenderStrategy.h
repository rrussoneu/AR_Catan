//
// Created by Raphael Russo on 9/24/24.
//

#ifndef AR_SETTLERS_OPENCVRENDERSTRATEGY_H
#define AR_SETTLERS_OPENCVRENDERSTRATEGY_H
#pragma once
#include "RenderStrategy.h"

class OpenCVRenderStrategy : public RenderStrategy {
public:

    // Reference about thread safe and general c++ singleton https://stackoverflow.com/questions/1008019/how-do-you-implement-the-singleton-design-pattern
    OpenCVRenderStrategy(const OpenCVRenderStrategy&) = delete;
    OpenCVRenderStrategy &operator=(const OpenCVRenderStrategy&) = delete;

    static OpenCVRenderStrategy &getInstance() {
        static OpenCVRenderStrategy instance; // Thread safe from c++ 11 on
        return instance;
    }
    void render(ArucoObject *object, cv::Mat &frame, const std::vector<cv::Point2f> &markerCorners, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const std::vector<double> &distCoeffs, const cv::Mat &cameraMatrix) override {
        std::vector<cv::Point2f> imgPoints; // 2D calculated points to draw
        std::vector<std::vector<cv::Point3f>> polys = object->getPolygons();
        for (int i = 0; i < polys.size(); ++i) {
            cv::projectPoints(polys.at(i), rvec, tvec, cameraMatrix, distCoeffs, imgPoints); // Solve
            cv::Scalar color = object->getColors().at(i);
            for (int j = 0; j < polys.at(i).size() - 1; ++j) {
                cv::line(frame, imgPoints.at(j), imgPoints.at(j + 1), color, 20);

            }
            cv::line(frame, imgPoints.at(imgPoints.size() - 1), imgPoints.at(0), color, 20);
        }


    }

private:
    OpenCVRenderStrategy() = default;
    ~OpenCVRenderStrategy() = default;
};
#endif //AR_SETTLERS_OPENCVRENDERSTRATEGY_H
