//
// Created by Raphael Russo on 9/22/24.
// A class for rtsp input
//

#ifndef AR_SETTLERS_RTSPINPUT_H
#define AR_SETTLERS_RTSPINPUT_H

#pragma once
#include "VideoInput.h"
#include <opencv2/videoio.hpp>

class RTSPInput : public VideoInput {
public:
    RTSPInput(const std::string& rtspUrl) : rtspUrl(rtspUrl) {}
    ~RTSPInput() { stopStream(); }

    bool startStream() override {
        if (!rtspStream.open(rtspUrl)) {
            return false;
        }
        isStreaming = true;
        return true;
    }


    void stopStream() override {
        if (isStreaming) {
            rtspStream.release();
            isStreaming = false;
        }
    }


    bool getFrame(cv::Mat &frame) override {
        if (!isStreaming || !rtspStream.isOpened()) {
            return false;
        }
        rtspStream >> frame;
        return !frame.empty();
    }

private:
    std::string rtspUrl;
    cv::VideoCapture rtspStream;
};

#endif //AR_SETTLERS_RTSPINPUT_H
