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
#include "Video/RTSPInput.h"
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


    // https://doc.qt.io/qt-6/signalsandslots.html Useful reference
public slots:
    void processFrame(const cv::Mat &frame);
    void switchToCamera();
    void switchToRTSP(const QString &rtspUrl);

    // Slots for view interaction
    void rollDice();
    void finishGame();
    void updatePlayerUsername(const QString &color, const QString &username);
    void incrementPlayerScore(const QString &color);
    void decrementPlayerScore(const QString &color);
    void onFrameProcessed(const cv::Mat &frame);

signals:
    // Signal to send processed frame to View
    void frameReady(const cv::Mat &frame);



    // Signal for dice roll
    void diceRolled(int roll);

    // Signal for player update
    //void playerUpdated(const QString& color, const Player& player);
    void playerUpdated(const QMap<QString, QVariant> &playerInfo);


private:
    GameModel *model;
    VideoInput *currentInput;  // Ptr for curr source
    VideoCaptureThread *captureThread;
    ProcessingThread *processingThread;

    CameraInput *cameraInput;  // Built in cam
    RTSPInput *rtspInput;      // RPi input --- COME BACK TO THIS LATER WHEN I HAVE THE RASPBERRY PI ON ME

    void stopCurrentInput();

    void runCommand(std::unique_ptr<Command> command) {
        QMap<QString, QVariant> result = command->run();
        if (!result.isEmpty()) {
            emit playerUpdated(result);
        }
    }


};


#endif //AR_SETTLERS_CONTROLLER_H
