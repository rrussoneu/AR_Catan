//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_PROCESSINGTHREAD_H
#define AR_SETTLERS_PROCESSINGTHREAD_H
#pragma once

#include <QThread>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include "../../Model/GameModel.h"

class ProcessingThread : public QThread {
Q_OBJECT
public:
    ProcessingThread(GameModel *model, QObject *parent = nullptr);
    ~ProcessingThread();

    void enqueueFrame(const cv::Mat& frame);

signals:
    void frameProcessed(const cv::Mat& frame);

protected:
    void run() override;

private:
    GameModel *model;
    QMutex mutex;
    QQueue<cv::Mat> frameQueue;
    QWaitCondition frameAvailable;

    bool stopThread;

    // Detection stuff
    cv::aruco::Dictionary dictionary; // Dictionary of the markers
    cv::aruco::DetectorParameters detectorParams;
    cv::aruco::ArucoDetector detector;

    void processFrame(const cv::Mat &frame);

    cv::Mat cameraMatrix;
    std::vector<double> distCoeffs;



};


#endif //AR_SETTLERS_PROCESSINGTHREAD_H
