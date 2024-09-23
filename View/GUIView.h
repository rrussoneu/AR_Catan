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
#include "../Controller/Controller.h"

class GUIView : public QWidget {
Q_OBJECT

public:
    GUIView(Controller *controller, QWidget* parent = nullptr);
    ~GUIView();


public slots:
    void updateVideoFeed(const cv::Mat& frame); // Slot to update  video feed

private:
    QLabel* videoLabel;   // Video display
    Controller* controller;
};



#endif //AR_SETTLERS_GUIVIEW_H
