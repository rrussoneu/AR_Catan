//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_CONTROLLER_H
#define AR_SETTLERS_CONTROLLER_H
#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>
#include "Video/VideoInput.h"
#include "Video/VideoCaptureThread.h"
#include "../Model/GameModel.h"
#include "Video/RTSPInput.h"
#include "Video/CameraInput.h"


class Controller : public QObject {

    Q_OBJECT

public:
    Controller(GameModel *model, QObject *parent = nullptr);
    ~Controller();


    // https://doc.qt.io/qt-6/signalsandslots.html Useful reference
public slots:
    void processFrame(const cv::Mat& frame);
    void switchToCamera();
    void switchToRTSP(const QString& rtspUrl);

signals:
    // Signal to send processed frame to View
    void frameReady(const cv::Mat& frame);

private:
    GameModel *model;
    VideoInput *currentInput;  // Ptr for curr source
    VideoCaptureThread *captureThread;

    CameraInput *cameraInput;  // Built in cam
    RTSPInput *rtspInput;      // RPi input --- COME BACK TO THIS LATER WHEN I HAVE THE RASPBERRY PI ON ME

    void stopCurrentInput();

    // Method to detect markers and update the model
    // void markerdetect/modelupdate(cv::Mat &frame);

};


#endif //AR_SETTLERS_CONTROLLER_H
