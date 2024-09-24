//
// Created by Raphael Russo on 9/22/24.
//

#include "ProcessingThread.h"

#include <opencv2/aruco.hpp>

ProcessingThread::ProcessingThread(GameModel *model, QObject *parent) : QThread(parent), model(model), stopThread(false),
        dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_1000)), detectorParams(cv::aruco::DetectorParameters()),
        detector(dictionary, detectorParams){}

ProcessingThread::~ProcessingThread() {
    mutex.lock();
    stopThread = true;
    frameAvailable.wakeOne();
    mutex.unlock();
    wait();
}

void ProcessingThread::enqueueFrame(const cv::Mat &frame) {
    QMutexLocker locker(&mutex);
    frameQueue.enqueue(frame.clone());
    frameAvailable.wakeOne();
}

void ProcessingThread::run() {
    while (true) {
        cv::Mat frame;
        mutex.lock();
        if (frameQueue.isEmpty()) {
            frameAvailable.wait(&mutex);
        }
        if (stopThread) {
            mutex.unlock();
            break;
        }
        if (!frameQueue.isEmpty()) {
            frame = frameQueue.dequeue();
        }
        mutex.unlock();
        if (!frame.empty()) {
            processFrame(frame);
        }
    }
}

void ProcessingThread::processFrame(const cv::Mat &frame) {

    // Detect markers
    cv::Mat processedFrame = frame.clone();
    std::vector<int> markerIDs;
    std::vector<std::vector<cv::Point2f>> markerCorners;
    std::vector<std::vector<cv::Point2f>> rejectedCandidates;

    detector.detectMarkers(processedFrame, markerCorners, markerIDs, rejectedCandidates);

    // Render objects based on the markers detected
    if (!markerIDs.empty()) {
        for (size_t i = 0; i < markerIDs.size(); ++i) {
            int markerID = markerIDs[i];
            ARObject* arObject = model->getObject(markerID);
            if (arObject) {
                arObject->render(processedFrame);
            }
        }
    }

    // Emit processed frame
    emit frameProcessed(processedFrame);

}