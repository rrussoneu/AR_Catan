//
// Created by Raphael Russo on 9/22/24.
//

#include "Controller.h"
#include "Commands/UpdateUsernameCommand.h"
#include "Commands/IncrementScoreCommand.h"
#include "Commands/DecrementScoreCommand.h"
#include "../Model/Database/DatabaseManager.h"
#include <opencv2/aruco.hpp>
#include <QInputDialog>
#include <QLineEdit>

Controller::Controller(GameModel *model, QObject *parent)
        : QObject(parent), model(model), currentInput(nullptr), captureThread(nullptr),
          cameraInput(new CameraInput()), rpInput(nullptr), processingThread(std::make_unique<ProcessingThread>(model, nullptr)) {
    // Initialize players
    QStringList colors = {"blue", "red", "orange", "white"};
    for (const QString& color : colors) {
        model->addPlayer(color, Player());
    }

    connect(processingThread.get(), &ProcessingThread::frameProcessed, this, &Controller::onFrameProcessed);
    connect(model, &MessageEmitter::sendError, this, &Controller::handleError);
    connect(processingThread.get(), &ProcessingThread::errorOccurred, this, &Controller::handleError);
    connect(&DatabaseManager::getInstance(), &MessageEmitter::sendError, this, &Controller::handleError);

    processingThread->start();

    switchToCamera();
}

Controller::~Controller() {
    stopCurrentInput();
    delete cameraInput;
    if (rpInput) delete rpInput;
    if (processingThread) {
        processingThread->stop();
        processingThread->wait();
    }
}

void Controller::stopCurrentInput() {
    if (captureThread) {
        captureThread->stop();
        captureThread->wait();
    }
    if (currentInput) {
        currentInput->stopStream();
        currentInput = nullptr;
    }
}

void Controller::switchToCamera() {
    stopCurrentInput();
    getParams();

    if (!cameraInput->startStream()) {
        emit handleError("Failed to open default camera");
        return;
    }

    currentInput = cameraInput;
    captureThread = std::make_unique<VideoCaptureThread>(currentInput);
    connect(captureThread.get(), &VideoCaptureThread::frameCaptured, this, &Controller::processFrame);
    captureThread->start();
}

void Controller::switchToURL(const QString &rtspUrl) {
    stopCurrentInput();

    getParams();

    if (rpInput) {
        delete rpInput;
        rpInput = nullptr;
    }

    rpInput = new NetworkInput(rtspUrl.toStdString());

    if (!rpInput->startStream()) {
        emit handleError("Remote video connection failed.");
        return;
    }

    currentInput = rpInput;
    captureThread = std::make_unique<VideoCaptureThread>(currentInput);
    connect(captureThread.get(), &VideoCaptureThread::frameCaptured, this, &Controller::processFrame);
    captureThread->start();
}

void Controller::getParams() {
    // Prompt the user for a file path

    // Move this to View logic later
    QString camParamPath = QInputDialog::getText(nullptr, "Enter Camera Parameter File Path",
                                                 "Camera Parameter File Path:",
                                                 QLineEdit::Normal, "camera_calibration_setting.xml");
    if (camParamPath.isEmpty()) {
        // Default file if no input is provided
        camParamPath = "camera_calibration_setting.xml";
    }

    // Pass the file path to the processing thread to init with the correct camera parameters
    processingThread->setCalibrationFilePath(camParamPath);
}

void Controller::processFrame(const cv::Mat &frame) {
    // Enqueue frame for processing
    if (processingThread) {
        processingThread->enqueueFrame(frame);
    }
}

void Controller::onFrameProcessed(const cv::Mat &frame) {
    // Emit processed frame to view
    emit frameReady(frame);
}

void Controller::rollDice() {
    int roll = model->rollDice();
    emit diceRolled(roll);
}

void Controller::finishGame() {
    model->finishGame();
}

void Controller::updatePlayerUsername(const QString &color, const QString &username) {
    auto updateCommand = std::make_unique<UpdateUsernameCommand>(model, color, username);
    // Store a raw pointer to the command object
    Command* rawCommandPtr = updateCommand.get();
    // Connect using the raw pointer to pass along a potential error
    connect(rawCommandPtr, &MessageEmitter::sendError, this, &Controller::handleError);
    runCommand(std::move(updateCommand));
}

void Controller::incrementPlayerScore(const QString &color) {
    runCommand(std::make_unique<IncrementScoreCommand>(model, color));
}

void Controller::decrementPlayerScore(const QString &color) {
    runCommand(std::make_unique<DecrementScoreCommand>(model, color));
}

void Controller::handleError(const QString &message) {
    // Emit the error to the view
    emit displayError(message);
}

void Controller::endProcessing() {
    // Handle the two other threads

    if (captureThread) {
        captureThread->stop();  // Stop video thread
        captureThread->wait();  // Wait for the thread to finish
    }
    if (processingThread) {
        processingThread->disconnect(); // Disconnect any signals
        processingThread->stop();  // Stop the processing thread
        processingThread->wait();  // Wait for the thread to finish
    }
}
