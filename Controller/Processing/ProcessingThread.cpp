//
// Created by Raphael Russo on 9/22/24.
//

#include "ProcessingThread.h"
#include "../../Config.h"

#include <opencv2/aruco.hpp>

ProcessingThread::ProcessingThread(GameModel *model, QObject *parent) : QThread(parent), model(model), stopThread(false),
        dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_1000)), detectorParams(cv::aruco::DetectorParameters()),
        detector(dictionary, detectorParams){

        try {
            cv::FileStorage fs("camera_calibration_setting.xml", cv::FileStorage::READ);
            if (fs.isOpened()) {
                fs["camera_matrix"] >> cameraMatrix;
                fs["distortion_coefficients"] >> distCoeffs;
                fs.release();
            }
        } catch (int e) {
            printf("Error with loading calibration");
        }


}

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
    /*
    cv::Mat obj_points(4, 1, CV_32FC3);
    float marker_length = .027; // measured on printed markers
    obj_points.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-marker_length/2.f, marker_length/2.f, 0);
    obj_points.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(marker_length/2.f, marker_length/2.f, 0);
    obj_points.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(marker_length/2.f, -marker_length/2.f, 0);
    obj_points.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-marker_length/2.f, -marker_length/2.f, 0);

    */



    // Render objects based on the markers detected
    if (!markerIDs.empty()) {

        cv::aruco::drawDetectedMarkers(processedFrame, markerCorners, markerIDs);

        std::vector<cv::Vec3d> rvecs(markerCorners.size()), tvecs(markerCorners.size()); // rotation, translation vectors
        cv::Mat obj_points(4, 1, CV_32FC3);
        float marker_length = Config::getInstance().getMarkerLength();
        obj_points.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-marker_length/2.f, marker_length/2.f, 0);
        obj_points.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(marker_length/2.f, marker_length/2.f, 0);
        obj_points.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(marker_length/2.f, -marker_length/2.f, 0);
        obj_points.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-marker_length/2.f, -marker_length/2.f, 0);

        for (size_t i = 0; i < markerIDs.size(); ++i) {
            int markerID = markerIDs[i];
            ARObject* arObject = model->getObject(markerID);
            solvePnP(obj_points, markerCorners.at(i), cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i), cv::SOLVEPNP_ITERATIVE);

            cv::drawFrameAxes(processedFrame, cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i), marker_length, 2);
            if (arObject) {
                arObject->render(processedFrame, markerCorners.at(i), rvecs.at(i), tvecs.at(i), distCoeffs, cameraMatrix);
            }
        }
    }

    // Emit processed frame
    emit frameProcessed(processedFrame);

}