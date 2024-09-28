//
// Created by Raphael Russo on 9/22/24.
//

#include "ProcessingThread.h"
#include "../../Config.h"
#include "../../AR_Objs/Rendering/OpenGLRenderStrategy.h"
#include <QOpenGLFramebufferObject>
#include <QDebug>

#include <opencv2/aruco.hpp>
#include <QOpenGLFunctions>

ProcessingThread::ProcessingThread(GameModel *model, QObject *parent) : QThread(parent), model(model), stopThread(false),
        dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_1000)), detectorParams(cv::aruco::DetectorParameters()),
        detector(dictionary, detectorParams), glContext(nullptr), offScreenSurface(nullptr){

        try {
            cv::FileStorage fs("camera_calibration_setting.xml", cv::FileStorage::READ);
            if (fs.isOpened()) {
                fs["camera_matrix"] >> cameraMatrix;
                fs["distortion_coefficients"] >> distCoeffs;
                fs.release();
            } else {
                printf("Calibration file opening failed.");
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

    // Clean up OpenGL resources
    if (glContext) {
        glContext->makeCurrent(offScreenSurface);
        glContext->doneCurrent();
        delete glContext;
    }
    if (offScreenSurface) {
        delete offScreenSurface;
    }

    if (renderStrategy) {
        delete renderStrategy;
        renderStrategy = nullptr;
    }
}

void ProcessingThread::enqueueFrame(const cv::Mat &frame) {
    QMutexLocker locker(&mutex);
    const int MAX_QUEUE_SIZE = 5; // Adjust as needed
    if (frameQueue.size() < MAX_QUEUE_SIZE) {
        frameQueue.enqueue(frame.clone());
        frameAvailable.wakeOne();
    } else {
        // Replace the oldest frame
        frameQueue.dequeue();
        frameQueue.enqueue(frame.clone());
        frameAvailable.wakeOne();
    }
}

void ProcessingThread::run() {
    // First need OpenGL init with context and offscreen surface

    // Create OpenGL context and offscreen surface
    glContext = new QOpenGLContext();
    QSurfaceFormat format;
    format.setVersion(3, 3); // OpenGL 3.3
    format.setProfile(QSurfaceFormat::CoreProfile); // Use core
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    glContext->setFormat(format);

    if (!glContext->create()) {
        qWarning() << "Failed to create OpenGL context";
        return;
    }

    offScreenSurface = new QOffscreenSurface();
    offScreenSurface->setFormat(glContext->format());
    offScreenSurface->create();

    if (!offScreenSurface->isValid()) {
        qWarning() << "Offscreen surface is not valid";
        return;
    }

    if (!glContext->makeCurrent(offScreenSurface)) {
        qWarning() << "Failed to make OpenGL context current";
        return;
    }

    // Init OpenGL functions
    QOpenGLFunctions *glFunctions = glContext->functions();
    glFunctions->initializeOpenGLFunctions();

    // Need first frame for info for fbo
    cv::Mat firstFrame;
    {
        QMutexLocker locker(&mutex);
        while (frameQueue.isEmpty() && !stopThread) {
            frameAvailable.wait(&mutex);
        }
        if (stopThread) {
            return;
        }
        firstFrame = frameQueue.dequeue();
    }

    if (firstFrame.empty()) {
        qWarning() << "Failed to retrieve the first frame";
        return;
    }

    int frameWidth = firstFrame.cols;
    int frameHeight = firstFrame.rows;

    // Create and initialize OpenGLRenderStrategy with frame dimensions
    renderStrategy = new OpenGLRenderStrategy(glContext);
    if (!renderStrategy->initialize(frameWidth, frameHeight)) {
        qWarning() << "Failed to initialize OpenGL render strategy";
        return;
    }

    // Process the first frame
    processFrame(firstFrame);

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

    // OpenGL set up fbo
    static_cast<OpenGLRenderStrategy*>(renderStrategy) ->prepareForRendering(processedFrame);


    // Render objects based on the markers detected
    if (!markerIDs.empty()) {
        std::vector<ARObject*> detectedObjects;
        cv::aruco::drawDetectedMarkers(processedFrame, markerCorners, markerIDs);

        std::vector<cv::Vec3d> rvecs(markerCorners.size()), tvecs(markerCorners.size()); // Rotation, translation vectors
        cv::Mat obj_points(4, 1, CV_32FC3);
        float marker_length = Config::getInstance().getMarkerLength();
        obj_points.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-marker_length/2.f, marker_length/2.f, 0);
        obj_points.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(marker_length/2.f, marker_length/2.f, 0);
        obj_points.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(marker_length/2.f, -marker_length/2.f, 0);
        obj_points.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-marker_length/2.f, -marker_length/2.f, 0);

        for (size_t i = 0; i < markerIDs.size(); ++i) {
            int markerID = markerIDs[i];
            ARObject* arObject = model->getObject(markerID);

            if (!arObject) {
                continue;
            }

            if (arObject->getRenderStrategy() == nullptr) {
                // Assign the OpenGLRenderStrategy
                arObject->setRenderStrategy(renderStrategy);
            }

            solvePnP(obj_points, markerCorners.at(i), cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i), cv::SOLVEPNP_ITERATIVE);

            cv::drawFrameAxes(processedFrame, cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i), marker_length, 2);


            if (arObject->getRenderStrategy()->getType() == 1) {
                arObject->render(processedFrame, markerCorners.at(i), rvecs.at(i), tvecs.at(i), distCoeffs, cameraMatrix);
            }else {
                arObject->render(processedFrame, markerCorners.at(i), rvecs.at(i), tvecs.at(i), distCoeffs, cameraMatrix);
            }


        }

        // Combine the frames
        static_cast<OpenGLRenderStrategy*>(renderStrategy)->finalizeRendering(processedFrame);

    }

    // Emit processed frame
    emit frameProcessed(processedFrame);



}