//
// Created by Raphael Russo on 9/22/24.
//

#include "ProcessingThread.h"
#include "../../Config.h"
#include "../../AR_Objs/Rendering/OpenGLRenderStrategy.h"
#include <QOpenGLFramebufferObject>

#include <opencv2/aruco.hpp>
#include <QOpenGLFunctions>

ProcessingThread::ProcessingThread(GameModel *model, QObject *parent) : QThread(parent), model(model), stopThread(false),
        dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_1000)), detectorParams(cv::aruco::DetectorParameters()),
        detector(dictionary, detectorParams), glContext(nullptr), offScreenSurface(nullptr){
}

ProcessingThread::~ProcessingThread() {
    // Stop thread and wait for thread to finish
    stop();
    wait(); // Wait for thread to finish executing

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

void ProcessingThread::stop() {
    QMutexLocker locker(&mutex); // Lock mutex for thread safety
    stopThread = true;
    frameAvailable.wakeOne();  // Wake the thread if it's waiting for frames
}

void ProcessingThread::enqueueFrame(const cv::Mat &frame) {
    QMutexLocker locker(&mutex); // Need thread safe queue access
    const int MAX_QUEUE_SIZE = 5; // Adjust as needed - five seems to be good for now
    if (frameQueue.size() < MAX_QUEUE_SIZE) {
        // Add a copy of the frame and notify thread avaiable
        frameQueue.enqueue(frame.clone());
        frameAvailable.wakeOne();
    } else {
        // Replace the oldest frame if full
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
    // Buffer sizes for rendering
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    glContext->setFormat(format);

    if (!glContext->create()) {
        emit errorOccurred("Failed to create OpenGL context");
        return;
    }

    // Offscreen surface for rendering objects
    offScreenSurface = new QOffscreenSurface();
    offScreenSurface->setFormat(glContext->format());
    offScreenSurface->create();

    if (!offScreenSurface->isValid()) {
        emit errorOccurred("Offscreen surface not valid");
        return;
    }

    if (!glContext->makeCurrent(offScreenSurface)) {
        emit errorOccurred("Failed to make OpenGL context current");
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
            frameAvailable.wait(&mutex); // Wait for enqueued frame
        }
        if (stopThread) {
            return;
        }
        firstFrame = frameQueue.dequeue();
    }

    if (firstFrame.empty()) {
        emit errorOccurred("Failed to get first frame");
        return;
    }

    int frameWidth = firstFrame.cols;
    int frameHeight = firstFrame.rows;

    // Create and initialize OpenGLRenderStrategy with frame dimensions
    renderStrategy = new OpenGLRenderStrategy();
    if (!renderStrategy->initialize(frameWidth, frameHeight)) {
        emit errorOccurred("Failed to initialize OpenGL render strategy");
    }

    // Process the first frame
    processFrame(firstFrame);

    // Main loop for processing
    while (true) {
        cv::Mat frame;
        mutex.lock();
        // Wait for new frame if queue is empty
        if (frameQueue.isEmpty()) {
            frameAvailable.wait(&mutex);
        }
        // Exit loop if stop flag
        if (stopThread) {
            mutex.unlock();
            break;
        }
        // Get next frame
        if (!frameQueue.isEmpty()) {
            frame = frameQueue.dequeue();
        }
        mutex.unlock();
        // Process it
        if (!frame.empty()) {
            processFrame(frame);
        }
    }
}

/**
 * Processes a frame for ArUco markers and renders virtual objects based on the marker's ID and rendering strategy
 * @param frame The frame to process from the video feed
 */
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


        float marker_length = Config::getInstance().getMarkerLength();
        // Marker points for pose
        cv::Mat objPoints(4, 1, CV_32FC3);
        objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-marker_length / 2.f, marker_length / 2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(marker_length / 2.f, marker_length / 2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(marker_length / 2.f, -marker_length / 2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-marker_length / 2.f, -marker_length / 2.f, 0);

        // For every found marker, solve pose and render based on strategy
        for (size_t i = 0; i < markerIDs.size(); ++i) {

            int markerID = markerIDs[i];
            ARObject* arObject = model->getObject(markerID);

            // Continue if no object for ID
            if (!arObject) {
                continue;
            }

            // If strategy was null then it needs an OpenGL strategy, add an enum for this later, this is just a dirty check for now
            if (arObject->getRenderStrategy() == nullptr) {
                // Assign the OpenGLRenderStrategy
                arObject->setRenderStrategy(renderStrategy);
            }

            // Solve for pose
            solvePnP(objPoints, markerCorners.at(i), cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i), cv::SOLVEPNP_ITERATIVE);

            // Uncomment to view the axes overlayed on each marker
            // cv::drawFrameAxes(processedFrame, cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i), marker_length, 2);

            // Render based on render strategy
            arObject->render(processedFrame, markerCorners.at(i), rvecs.at(i), tvecs.at(i), distCoeffs, cameraMatrix);

        }

        // Combine the frames
        static_cast<OpenGLRenderStrategy*>(renderStrategy)->finalizeRendering(processedFrame);

    }

    emit frameProcessed(processedFrame);

}

void ProcessingThread::setCalibrationFilePath(const QString &filePath) {
    // Load the camera calibration info from given file path
    try {
        cv::FileStorage fs(filePath.toStdString(), cv::FileStorage::READ);
        if (fs.isOpened()) {
            fs["camera_matrix"] >> cameraMatrix;
            fs["distortion_coefficients"] >> distCoeffs;
            fs.release();
        } else {
            emit errorOccurred("Calibration file opening failed - using default");

            // Default file
            cv::FileStorage fallbackFs("camera_calibration_setting.xml", cv::FileStorage::READ);
            if (fallbackFs.isOpened()) {
                fallbackFs["camera_matrix"] >> cameraMatrix;
                fallbackFs["distortion_coefficients"] >> distCoeffs;
                fallbackFs.release();
            }
        }
    } catch (int e) {
        emit errorOccurred("Calibration info error - using default");
    }
}