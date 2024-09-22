//
// Created by Raphael Russo on 9/22/24.
// Header for class for View using Qt GUI
//

#ifndef AR_SETTLERS_GUIVIEW_H
#define AR_SETTLERS_GUIVIEW_H

#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "../Video/CameraInput.h"
#include "../Video/RTSPInput.h"
#include "../Video/VideoCaptureThread.h"

class GUIView : public QWidget {
Q_OBJECT

public:
    GUIView(QWidget* parent = nullptr);
    ~GUIView();


public slots: // https://doc.qt.io/qt-6/signalsandslots.html
    void updateVideoFeed(const cv::Mat& frame);   // Slot to update the video feed
    void switchToCamera();   // Slot to switch to built-in camera
    void switchToRTSP();     // Slot to switch to Raspberry Pi RTSP stream

private:
    QLabel *videoLabel;  // Video diplay
    VideoInput *currentInput;  // Ptr for curr source
    VideoCaptureThread *captureThread;

    CameraInput *cameraInput;  // Built in cam

    RTSPInput *rtspInput;      // RPi input --- COME BACK TO THIS LATER WHEN I HAVE THE RASPBERRY PI ON ME
};



#endif //AR_SETTLERS_GUIVIEW_H
