//
// Created by Raphael Russo on 9/22/24.
//

#include "Controller.h"
#include <opencv2/aruco.hpp>

Controller::Controller(GameModel* model, QObject* parent)
        : QObject(parent), model(model), currentInput(nullptr), captureThread(nullptr),
          cameraInput(new CameraInput()), rtspInput(nullptr) {
    // Default to camera input
    switchToCamera();
}

Controller::~Controller() {
    stopCurrentInput();
    delete cameraInput;
    if (rtspInput) delete rtspInput;
}

void Controller::stopCurrentInput() {
    if (captureThread) {
        captureThread->stop();
        captureThread->wait();
        delete captureThread;
        captureThread = nullptr;
    }
    if (currentInput) {
        currentInput->stopStream();
        currentInput = nullptr;
    }
}

void Controller::switchToCamera() {
    stopCurrentInput();

    if (!cameraInput->startStream()) {
        qWarning("Failed to open default camera");
        return;
    }

    currentInput = cameraInput;
    captureThread = new VideoCaptureThread(currentInput);
    connect(captureThread, &VideoCaptureThread::frameCaptured, this, &Controller::processFrame);
    captureThread->start();
}

void Controller::switchToRTSP(const QString& rtspUrl) {
    stopCurrentInput();

    if (rtspInput) {
        delete rtspInput;
    }
    rtspInput = new RTSPInput(rtspUrl.toStdString());

    if (!rtspInput->startStream()) {
        qWarning("RTSP connection failed.");
        return;
    }

    currentInput = rtspInput;
    captureThread = new VideoCaptureThread(currentInput);
    connect(captureThread, &VideoCaptureThread::frameCaptured, this, &Controller::processFrame);
    captureThread->start();
}

void Controller::processFrame(const cv::Mat& frame) {
    cv::Mat processedFrame = frame.clone();

    // Perform marker detection and update the Model
    //marker and model stuff (processedFrame);

    // Emit the processed frame to the View
    emit frameReady(processedFrame);
}
