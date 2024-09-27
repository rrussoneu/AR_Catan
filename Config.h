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

    cv::Mat getCameraMatrix() const {
        std::lock_guard<std::mutex> lock(configMutex);
        return cameraMatrix;
    }

    std::vector<double> getDistCoeffs() const {
        std::lock_guard<std::mutex> lock(configMutex);
        return distCoeffs;
    }


    void setMarkerLength(float length) {
        std::lock_guard<std::mutex> lock(configMutex);
        markerLength = length;
    }




private:

    Config() : markerLength(0.038f) {
        try {
            cv::FileStorage fs("camera_calibration_setting.xml", cv::FileStorage::READ);
            if (fs.isOpened()) {
                fs["camera_matrix"] >> cameraMatrix;
                fs["distortion_coefficients"] >> distCoeffs;
                fs.release();
            }
        } catch (int e) {
            printf("Error with loading calibration");
        }
    }


    float markerLength;
    cv::Mat cameraMatrix;
    std::vector<double> distCoeffs;




    mutable std::mutex configMutex;
};

#endif //AR_SETTLERS_CONFIG_H
