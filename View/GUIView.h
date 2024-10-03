//
// Created by Raphael Russo on 9/22/24.
// Header for class for View using Qt GUI
//

#ifndef AR_SETTLERS_GUIVIEW_H
#define AR_SETTLERS_GUIVIEW_H

#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QGroupBox>
#include "../Controller/Controller.h"

class GUIView : public QWidget {
Q_OBJECT

public:
    GUIView(Controller *controller, QWidget* parent = nullptr);
    ~GUIView();


public slots:
    void updateVideoFeed(const cv::Mat& frame); // Slot to update  video feed
    void updateDiceRoll(int roll);
    void updatePlayerInfo(const QMap<QString, QVariant>& playerInfo);

    void displayError(const QString &message);

private:
    QLabel *videoLabel;   // Video display
    Controller *controller;

    // Roll dice
    QPushButton *rollDiceButton;
    QLabel *diceRollLabel;

    // Finish game
    QPushButton *finishGameButton;

    // Player panels - each player color gets one
    QGroupBox* createPlayerPanel(const QString& color);
    std::unordered_map<QString, QGroupBox*> playerPanels;

};



#endif //AR_SETTLERS_GUIVIEW_H
