//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_AROBJECT_H
#define AR_SETTLERS_AROBJECT_H

#pragma once
#include <opencv2/opencv.hpp>
#include "Rendering/RenderStrategy.h"

class RenderStrategy;

class ARObject {
public:
    ARObject(RenderStrategy *strategy = nullptr, const std::string &objectName = "") : renderStrategy(strategy), name(objectName) {}
    virtual ~ARObject() = default;


    virtual void render(cv::Mat &frame, const std::vector<cv::Point2f> &markerCorners, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const std::vector<double> &distCoeffs, const cv::Mat &cameraMatrix) = 0;


    void setRenderStrategy(RenderStrategy* strategy) {
        renderStrategy = strategy;
    }


    void setPose(const cv::Vec3d& r, const cv::Vec3d& t) {
        rvec = r;
        tvec = t;
    }

    cv::Vec3d getRvec() const { return rvec; }
    cv::Vec3d getTvec() const { return tvec; }

    void setMarkerCorners(const std::vector<cv::Point2f> corners) {
        markerCorners = corners;
    }
    std::vector<cv::Point2f> getMarkerCorners() const { return markerCorners; }

    const std::string& getName() {
        const std::string defaultName = "brick"; // Just default to a brick for simplicity
        return name.empty() ? defaultName : name;

    }

    RenderStrategy* getRenderStrategy() const {
        return renderStrategy;
    }

    virtual const std::vector<std::vector<cv::Point3f>>& getPolygons() const = 0;
    virtual const std::vector<cv::Scalar>& getColors() const = 0;
protected:
    RenderStrategy *renderStrategy;

private:
    cv::Vec3d rvec;
    cv::Vec3d tvec;
    std::vector<cv::Point2f> markerCorners;
    std::string name;
};
#endif //AR_SETTLERS_AROBJECT_H
