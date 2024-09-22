//
// Created by Raphael Russo on 9/22/24.
// GUI / View class implementation
//

#include "GUIView.h"



GUIView::GUIView(QWidget* parent) : QWidget(parent), currentInput(nullptr), captureThread(nullptr), rtspInput(nullptr) {

    QVBoxLayout *mainLayout = new QVBoxLayout;
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(640, 480);
    mainLayout->addWidget(videoLabel);

    // Switch video source buttons
    QPushButton *cameraButton = new QPushButton("Switch to Camera", this);
    QPushButton *rtspButton = new QPushButton("Switch to RTSP Stream", this);
    mainLayout->addWidget(cameraButton);
    mainLayout->addWidget(rtspButton);
    connect(cameraButton, &QPushButton::clicked, this, &GUIView::switchToCamera);
    connect(rtspButton, &QPushButton::clicked, this, &GUIView::switchToRTSP);

    /*
     *  To Do:
     *      Add some buttons for the users scores, dice rolls, etc.
     * */

    setLayout(mainLayout);
    cameraInput = new CameraInput();
    // Computer camera should be default
    switchToCamera();
}

GUIView::~GUIView() {
    // Thread handling
    if (captureThread) {
        captureThread->stop();
        captureThread->wait();
        delete captureThread;
    }
    delete cameraInput;
    if (rtspInput) delete rtspInput;
}

// Slot for computer camera
void GUIView::switchToCamera() {
    if (captureThread) {
        captureThread->stop();
        captureThread->wait();
        delete captureThread;
        captureThread = nullptr;
    }

    if (!cameraInput->startStream()) {
        qWarning("Failed to open default camera");
        return;
    }

    currentInput = cameraInput;
    captureThread = new VideoCaptureThread(currentInput);
    connect(captureThread, &VideoCaptureThread::frameCaptured, this, &GUIView::updateVideoFeed);
    captureThread->start();
}

// Slot to switch to the rpi rtsp stream
void GUIView::switchToRTSP() {
    if (captureThread) {
        captureThread->stop();
        captureThread->wait();
        delete captureThread;
        captureThread = nullptr;
    }

    if (!rtspInput) {
        rtspInput = new RTSPInput("rtsp://PUT_THE_URL_HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }



    if (!rtspInput->startStream()) {
        qWarning("RTSP connection failed.");
        return;
    }

    currentInput = rtspInput;
    captureThread = new VideoCaptureThread(currentInput);
    connect(captureThread, &VideoCaptureThread::frameCaptured, this, &GUIView::updateVideoFeed);
    captureThread->start();
}

// Update video feed slot -- capture thread calls
void GUIView::updateVideoFeed(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::Mat rgbFrame;
        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB); // OpenCV is BGR but Qt is RGB
        QImage qImg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        videoLabel->setPixmap(QPixmap::fromImage(qImg).scaled(videoLabel->size(), Qt::KeepAspectRatio));
    }
}

