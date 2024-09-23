//
// Created by Raphael Russo on 9/22/24.
// A class for the computer's camera's input
//

#ifndef AR_SETTLERS_CAMERAINPUT_H
#define AR_SETTLERS_CAMERAINPUT_H

#pragma once
#include "VideoInput.h"

class CameraInput : public VideoInput {
public:
    CameraInput() {}
    ~CameraInput() {stopStream();}

    bool startStream() override {
        if (!videoCapture.isOpened()) {
            if (!videoCapture.open(0)) {
                return false;
            }
        }
        isStreaming = true;
        return true;
    }

    void stopStream() override {
        if (isStreaming) {
            videoCapture.release(); // Camera needs to be freed
            isStreaming = false;
        }
    }

    bool getFrame(cv::Mat &frame) override {
        if (!isStreaming || !videoCapture.isOpened()) {
            return false;
        }
        videoCapture >> frame;
        return !frame.empty();
    }

private:
    cv::VideoCapture videoCapture;
};

#endif //AR_SETTLERS_CAMERAINPUT_H
