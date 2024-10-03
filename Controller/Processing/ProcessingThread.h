//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_PROCESSINGTHREAD_H
#define AR_SETTLERS_PROCESSINGTHREAD_H
#pragma once

#include <QThread>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <QQueue>
#include <QWaitCondition>
#include "../../Model/GameModel.h"

/**
 * A class to handle frame processing in its own thread
 */
class ProcessingThread : public QThread {
Q_OBJECT
public:
    /**
     * Constructor
     * @param model Game model with AR objects to render
     * @param parent Optional parent object for thread
     */
     ProcessingThread(GameModel *model, QObject *parent = nullptr);
    ~ProcessingThread();

    /**
     * Enqueues a frame to be processed
     * @param frame A frame from the video
     */
    void enqueueFrame(const cv::Mat& frame);

    /**
     * Stop processing and exit
     */
    void stop();

    /**
     * Set path for camera calibration/ parameters file
     * @param filePath The path
     */
    void setCalibrationFilePath(const QString &filePath);

public slots:
    void handleError(const QString &message);

signals:
    /**
     * Emits when a prame is processed and is ready for display
     * @param frame The frame that's ready
     */
    void frameProcessed(const cv::Mat& frame);

    // Signal for errors
    void errorOccurred(const QString &message);

protected:

    /**
     * Main processing loop (QThread override)
     * OpenGL init, frame processing, marker detection
     */
    void run() override;

private:

    GameModel *model;
    QMutex mutex; // Mutex for thread safe access to frame queue / shared data
    QQueue<cv::Mat> frameQueue; // The queue of frames to process
    QWaitCondition frameAvailable; // Condition whether new frame is available

    bool stopThread; // Flag for when thread should stop processing

    // Detection stuff
    cv::aruco::Dictionary dictionary; // Dictionary of the markers
    cv::aruco::DetectorParameters detectorParams;
    cv::aruco::ArucoDetector detector;

    /**
     * Processes a frame and renders the objects
     * @param frame The frame to process
     */
    void processFrame(const cv::Mat &frame);

    // Camera calibration data
    cv::Mat cameraMatrix; // Intrinsics
    std::vector<double> distCoeffs; // Distortion coefficients

    // OpenGL context and surface for offscreen rendering
    QOpenGLContext *glContext;
    QOffscreenSurface *offScreenSurface;

    // OpenGL rendering that the markers with this strategy have a pointer for
    RenderStrategy *renderStrategy;




};


#endif //AR_SETTLERS_PROCESSINGTHREAD_H
