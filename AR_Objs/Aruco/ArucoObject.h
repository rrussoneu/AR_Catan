//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ARUCOOBJECT_H
#define AR_SETTLERS_ARUCOOBJECT_H
#pragma once
#include "../ARObject.h"

#include "../../Config.h"
#include <opencv2/opencv.hpp>
#include <vector>

class ArucoObject : public ARObject {
public:
    ArucoObject(int markerID, RenderStrategy *strategy= nullptr, const std::string &objectName = "")
            : markerID(markerID), ARObject(strategy, objectName) {}

    virtual ~ArucoObject() = default;

    int getMarkerID() const { return markerID; }



    void render(cv::Mat &frame, const std::vector<cv::Point2f> &markerCorners, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const std::vector<double> &distCoeffs, const cv::Mat &cameraMatrix) override {
        if (renderStrategy) {
            renderStrategy->render(this, frame, markerCorners, rvec, tvec, distCoeffs, cameraMatrix);
        }
    }


    //virtual const std::vector<std::vector<cv::Point3f>>& getPolygons() const = 0;
    //virtual const std::vector<cv::Scalar>& getColors() const = 0;


    // Extra scale factor to make things more easily adjustable
    virtual void scalePoints(std::vector<std::vector<cv::Point3f>> *polygons, float factor = 1.0) {
        float markerLength = Config::getInstance().getMarkerLength() * factor;
        for (auto &polygon : *polygons) {
            for (auto &point : polygon) {
                point.x *= markerLength;
                point.y *= markerLength;
                point.z *= markerLength;
            }
        }
    }


protected:
    int markerID;


};
#endif //AR_SETTLERS_ARUCOOBJECT_H
