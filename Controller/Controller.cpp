//
// Created by Raphael Russo on 9/22/24.
//

#include "Controller.h"
#include "Commands/UpdateUsernameCommand.h"
#include "Commands/IncrementScoreCommand.h"
#include "Commands/DecrementScoreCommand.h"
#include <opencv2/aruco.hpp>
#include <QMessageBox>

Controller::Controller(GameModel *model, QObject *parent)
        : QObject(parent), model(model), currentInput(nullptr), captureThread(nullptr),
          cameraInput(new CameraInput()), rpInput(nullptr), processingThread(new ProcessingThread(model)) {
    // Initialize players
    QStringList colors = {"blue", "red", "orange", "white"};
    for (const QString& color : colors) {
        model->addPlayer(color, Player());
    }

    connect(processingThread, &ProcessingThread::frameProcessed, this, &Controller::onFrameProcessed);
    processingThread->start();

    switchToCamera();
}

Controller::~Controller() {
    stopCurrentInput();
    delete cameraInput;
    if (rpInput) delete rpInput;
}

void Controller::stopCurrentInput() {
    if (captureThread) {
        captureThread->stop();
        captureThread->wait();
        delete captureThread;
        captureThread = nullptr;
    }
    if (currentInput) {
        currentInput->stopStream();
        currentInput = nullptr;
    }
}

void Controller::switchToCamera() {
    stopCurrentInput();

    if (!cameraInput->startStream()) {
        qWarning("Failed to open default camera");
        return;
    }

    currentInput = cameraInput;
    captureThread = new VideoCaptureThread(currentInput);
    connect(captureThread, &VideoCaptureThread::frameCaptured, this, &Controller::processFrame);
    captureThread->start();
}

void Controller::switchToURL(const QString &rtspUrl) {
    stopCurrentInput();

    if (rpInput) {
        delete rpInput;
    }
    rpInput = new NetworkInput(rtspUrl.toStdString());

    if (!rpInput->startStream()) {
        qWarning("RTSP connection failed.");
        return;
    }

    currentInput = rpInput;
    captureThread = new VideoCaptureThread(currentInput);
    connect(captureThread, &VideoCaptureThread::frameCaptured, this, &Controller::processFrame);
    captureThread->start();
}

void Controller::processFrame(const cv::Mat &frame) {
    // Enqueue frame for processing
    processingThread->enqueueFrame(frame);
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
    runCommand(std::make_unique<UpdateUsernameCommand>(model, color, username));
}

void Controller::incrementPlayerScore(const QString &color) {
    runCommand(std::make_unique<IncrementScoreCommand>(model, color));
}

void Controller::decrementPlayerScore(const QString &color) {
    runCommand(std::make_unique<DecrementScoreCommand>(model, color));
}



