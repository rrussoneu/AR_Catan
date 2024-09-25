//
// Created by Raphael Russo on 9/25/24.
//

#ifndef AR_SETTLERS_CONFIG_H
#define AR_SETTLERS_CONFIG_H
#pragma once
#include <mutex>
#include <opencv2/opencv.hpp>


class Config {
public:

    Config(const Config&) = delete;
    Config &operator=(const Config&) = delete;


    static Config& getInstance() {
        static Config instance;
        return instance;
    }


    float getMarkerLength() const {
        std::lock_guard<std::mutex> lock(configMutex);
        return markerLength;
    }


    void setMarkerLength(float length) {
        std::lock_guard<std::mutex> lock(configMutex);
        markerLength = length;
    }


    void setCameraMatrix(const cv::Mat &matrix) {
        std::lock_guard<std::mutex> lock(configMutex);
        cameraMatrix = matrix.clone();
    }

    cv::Mat getCameraMatrix() const {
        std::lock_guard<std::mutex> lock(configMutex);
        return cameraMatrix.clone();
    }



private:

    Config() : markerLength(0.027f) {}


    float markerLength;
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;

    mutable std::mutex configMutex;
};

#endif //AR_SETTLERS_CONFIG_H
