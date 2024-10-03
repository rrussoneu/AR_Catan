//
// Created by Raphael Russo on 9/24/24.
//

#ifndef AR_SETTLERS_RENDERSTRATEGY_H
#define AR_SETTLERS_RENDERSTRATEGY_H
#pragma once


#include <opencv2/opencv.hpp>
#include "../ARObject.h"
#include "../../Model/MessageEmitter.h"
#include <QObject>

class ARObject;

class RenderStrategy : public MessageEmitter {
public:
    explicit RenderStrategy(QObject *parent = nullptr);
    virtual ~RenderStrategy() = default;
    virtual void render(ARObject *object, cv::Mat &frame,
                        const std::vector<cv::Point2f> &markerCorners,
                        const cv::Vec3d &rvec, const cv::Vec3d &tvec,
                        const std::vector<double> &distCoeffs,
                        const cv::Mat &cameraMatrix) = 0;
    virtual bool initialize(int frameWidth, int frameHeight) = 0;

    virtual int getType() = 0;

};
#endif //AR_SETTLERS_RENDERSTRATEGY_H
