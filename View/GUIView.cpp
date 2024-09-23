//
// Created by Raphael Russo on 9/22/24.
// GUI / View class implementation
//

#include "GUIView.h"
#include <QInputDialog>


GUIView::GUIView(Controller* controller, QWidget* parent)
        : QWidget(parent), controller(controller) {
    QVBoxLayout* mainLayout = new QVBoxLayout;
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(640, 480);
    mainLayout->addWidget(videoLabel);

    // Switch video source buttons
    QPushButton* cameraButton = new QPushButton("Switch to Camera", this);
    QPushButton* rtspButton = new QPushButton("Switch to RTSP Stream", this);
    mainLayout->addWidget(cameraButton);
    mainLayout->addWidget(rtspButton);

    // Connect buttons to controller slots
    connect(cameraButton, &QPushButton::clicked, controller, &Controller::switchToCamera);
    connect(rtspButton, &QPushButton::clicked, [this]() {
        QString rtspUrl = QInputDialog::getText(this, "Enter RTSP URL", "RTSP URL:");
        if (!rtspUrl.isEmpty()) {
            this->controller->switchToRTSP(rtspUrl);
        }
    });

    // Connect controllers frameReady signal to updateVideoFeed slot
    connect(controller, &Controller::frameReady, this, &GUIView::updateVideoFeed);

    setLayout(mainLayout);
}

GUIView::~GUIView() {
    // Nothing to delete
}

void GUIView::updateVideoFeed(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::Mat rgbFrame;
        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB); // Convert to RGB for Qt
        QImage qImg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        videoLabel->setPixmap(QPixmap::fromImage(qImg).scaled(videoLabel->size(), Qt::KeepAspectRatio));
    }
}
