//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_CONTROLLER_H
#define AR_SETTLERS_CONTROLLER_H
#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QtCore/QMap>
#include "Video/VideoInput.h"
#include "Video/VideoCaptureThread.h"
#include "../Model/GameModel.h"
#include "Video/NetworkInput.h"
#include "Video/CameraInput.h"
#include "Processing/ProcessingThread.h"
#include "Commands/Command.h"

struct PlayerInfo {
    QString color;
    QString username;
    int score;
    int wins;
    int gamesPlayed;
    double averageScore;
};


class Controller : public QObject {

    Q_OBJECT

public:
    Controller(GameModel *model, QObject *parent = nullptr);
    ~Controller();

    void endProcessing();

    // https://doc.qt.io/qt-6/signalsandslots.html Useful reference
public slots:
    void processFrame(const cv::Mat &frame);
    void switchToCamera();
    void switchToURL(const QString &rtspUrl);

    // Slots for view interaction
    void rollDice();
    void finishGame();
    void updatePlayerUsername(const QString &color, const QString &username);
    void incrementPlayerScore(const QString &color);
    void decrementPlayerScore(const QString &color);
    void onFrameProcessed(const cv::Mat &frame);
    //void onPromptEntered(const QString &input);

    // Slot for receiving error messages
    void handleError(const QString &message);



signals:
    // Signal to send processed frame to View
    void frameReady(const cv::Mat &frame);



    // Signal for dice roll
    void diceRolled(int roll);

    // Signal for player update
    //void playerUpdated(const QString& color, const Player& player);
    void playerUpdated(const QMap<QString, QVariant> &playerInfo);

    // Signal to errors to the view
    void displayError(const QString &message);

    // Signal to display normal messages to View
    void displayMessage(const QString &message);

    // Signal to send a prompt
    void displayPrompt(const QString &message);


private:
    GameModel *model;
    VideoInput *currentInput;  // Pointer for curr source
    VideoCaptureThread *captureThread;
    ProcessingThread *processingThread;


    CameraInput *cameraInput;  // Built in cam
    NetworkInput *rpInput; // Network input (Raspberry Pi)

    void stopCurrentInput();

    void getParams();

    void runCommand(std::unique_ptr<Command> command) {
        QMap<QString, QVariant> result = command->run();
        if (!result.isEmpty()) {
            emit playerUpdated(result);
        }
    }


};


#endif //AR_SETTLERS_CONTROLLER_H
