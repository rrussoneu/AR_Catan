//
// Created by Raphael Russo on 9/22/24.
// A class for a thread for video capturing
//

#ifndef AR_SETTLERS_VIDEOCAPTURETHREAD_H
#define AR_SETTLERS_VIDEOCAPTURETHREAD_H

#pragma once
#include <QThread>
#include <opencv2/opencv.hpp>
#include "VideoInput.h"

// https://doc.qt.io/qt-6/signalsandslots.html

class VideoCaptureThread : public QThread {
    Q_OBJECT
public:
    VideoCaptureThread(VideoInput *videoInput, QObject *parent = nullptr) : QThread(parent), videoInput(videoInput), stopThread(false) {}

    void run() override {
        cv::Mat frame;
        while (!stopThread) {
            if (videoInput->getFrame(frame)) {
                emit frameCaptured(frame);
            }
            msleep(30);
        }
    }

    void stop() {
        stopThread = true;
    }
signals:
    void frameCaptured(const cv::Mat &frame);

private:
    VideoInput *videoInput; // Both camera and network inputs depending on computer vs other camera
    bool stopThread;
};

#endif //AR_SETTLERS_VIDEOCAPTURETHREAD_H
