//
// Created by Raphael Russo on 9/22/24.
// A class for rtsp input
//

#ifndef AR_SETTLERS_NETWORKINPUT_H
#define AR_SETTLERS_NETWORKINPUT_H

#pragma once
#include "VideoInput.h"
#include <opencv2/videoio.hpp>

class NetworkInput : public VideoInput {
public:
    NetworkInput(const std::string& networkURL) : networkURL(networkURL) {}
    ~NetworkInput() { stopStream(); }

    bool startStream() override {
        if (!networkStream.open(networkURL)) {
            return false;
        }
        isStreaming = true;
        return true;
    }


    void stopStream() override {
        if (isStreaming) {
            networkStream.release();
            isStreaming = false;
        }
    }


    bool getFrame(cv::Mat &frame) override {
        if (!isStreaming || !networkStream.isOpened()) {
            return false;
        }
        networkStream >> frame;
        return !frame.empty();
    }

private:
    std::string networkURL;
    cv::VideoCapture networkStream;
};

#endif //AR_SETTLERS_NETWORKINPUT_H
