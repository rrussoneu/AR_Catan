//
// Created by Raphael Russo on 9/22/24.
// GUI / View class implementation
//

#include "GUIView.h"
#include <QInputDialog>


GUIView::GUIView(Controller* controller, QWidget* parent)
        : QWidget(parent), controller(controller) {
    QVBoxLayout* mainLayout = new QVBoxLayout;

    // Video
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(640, 480);
    mainLayout->addWidget(videoLabel);

    // Switch video source buttons and layout
    QHBoxLayout* videoButtonsLayout = new QHBoxLayout;
    QPushButton* cameraButton = new QPushButton("Switch to Camera", this);
    QPushButton* rtspButton = new QPushButton("Switch to RTSP Stream", this);
    videoButtonsLayout->addWidget(cameraButton);
    videoButtonsLayout->addWidget(rtspButton);
    mainLayout->addLayout(videoButtonsLayout);

    // Connect buttons to controller slots for video
    connect(cameraButton, &QPushButton::clicked, controller, &Controller::switchToCamera);
    connect(rtspButton, &QPushButton::clicked, [this]() {
        QString rtspUrl = QInputDialog::getText(this, "Enter RTSP URL", "RTSP URL:");
        if (!rtspUrl.isEmpty()) {
            this->controller->switchToRTSP(rtspUrl);
        }
    });

    // Rolling dice
    QHBoxLayout* diceLayout = new QHBoxLayout;
    rollDiceButton = new QPushButton("Roll Dice", this);
    diceRollLabel = new QLabel("Dice Roll: ", this);
    diceLayout->addWidget(rollDiceButton);
    diceLayout->addWidget(diceRollLabel);
    mainLayout->addLayout(diceLayout);
    connect(rollDiceButton, &QPushButton::clicked, controller, &Controller::rollDice);

    // Finish game button
    finishGameButton = new QPushButton("Finish Game", this);
    mainLayout->addWidget(finishGameButton);

    connect(finishGameButton, &QPushButton::clicked, controller, &Controller::finishGame);

    // Player panels
    QGridLayout* playersLayout = new QGridLayout;

    QStringList colors = {"blue", "red", "orange", "white"};
    int col = 0;
    for (const QString& color : colors) {
        QGroupBox* playerPanel = createPlayerPanel(color);
        playersLayout->addWidget(playerPanel, 0, col++);
        playerPanels[color] = playerPanel;
    }
    mainLayout->addLayout(playersLayout);


    // Connect controllers signals to slots here in view
    connect(controller, &Controller::frameReady, this, &GUIView::updateVideoFeed);
    connect(controller, &Controller::diceRolled, this, &GUIView::updateDiceRoll);
    connect(controller, &Controller::playerUpdated, this, &GUIView::updatePlayerInfo);

    setLayout(mainLayout);
}

GUIView::~GUIView() {
    // Nothing to delete
}

void GUIView::updateVideoFeed(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::Mat rgbFrame;
        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB); // Convert to RGB for Qt
        QImage qImg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        videoLabel->setPixmap(QPixmap::fromImage(qImg).scaled(videoLabel->size(), Qt::KeepAspectRatio));
    }
}

void GUIView::updateDiceRoll(int roll) {
    diceRollLabel->setText(QString("Dice Roll: %1").arg(roll));
}

/*

void GUIView::updatePlayerInfo(const QString& color, const Player& player) {
    QGroupBox* panel = playerPanels[color];
    if (panel) {
        QLineEdit* usernameEdit = panel->findChild<QLineEdit*>("usernameEdit");
        QLabel* scoreLabel = panel->findChild<QLabel*>("scoreLabel");
        QLabel* statsLabel = panel->findChild<QLabel*>("statsLabel");

        if (usernameEdit) {
            usernameEdit->setText(QString::fromStdString(player.getUsername()));
        }
        if (scoreLabel) {
            scoreLabel->setText(QString("Score: %1").arg(player.getScore()));
        }
        if (statsLabel) {
            statsLabel->setText(QString("Wins: %1\nGames Played: %2\nAvg Score: %3")
                                        .arg(player.getWins())
                                        .arg(player.getGamesPlayed())
                                        .arg(player.getAverageScore()));
        }
    }
}
 */

void GUIView::updatePlayerInfo(const QMap<QString, QVariant>& playerInfo) {
    QString color = playerInfo["color"].toString();
    QGroupBox* panel = playerPanels[color];
    if (panel) {
        QLineEdit* usernameEdit = panel->findChild<QLineEdit*>("usernameEdit");
        QLabel* scoreLabel = panel->findChild<QLabel*>("scoreLabel");
        QLabel* statsLabel = panel->findChild<QLabel*>("statsLabel");

        if (usernameEdit && playerInfo.contains("username")) {
            usernameEdit->setText(playerInfo["username"].toString());
        }
        if (scoreLabel && playerInfo.contains("score")) {
            scoreLabel->setText(QString("Score: %1").arg(playerInfo["score"].toInt()));
        }
        if (statsLabel) {
            QString stats = "Wins: %1\nGames Played: %2\nAvg Score: %3";
            stats = stats.arg(playerInfo.value("wins", 0).toInt())
                    .arg(playerInfo.value("gamesPlayed", 0).toInt())
                    .arg(playerInfo.value("averageScore", 0.0).toDouble(), 0, 'f', 2);
            statsLabel->setText(stats);
        }
    }
}

QGroupBox* GUIView::createPlayerPanel(const QString& color) {

    QGroupBox* groupBox = new QGroupBox(color.toUpper() + " PLAYER", this);
    //color.toLower();
    QVBoxLayout* vbox = new QVBoxLayout;

    // Username
    QLineEdit* usernameEdit = new QLineEdit("guest", this);
    usernameEdit->setObjectName("usernameEdit");
    usernameEdit->setFocusPolicy(Qt::ClickFocus);
    vbox->addWidget(new QLabel("Username:", this));
    vbox->addWidget(usernameEdit);


    // Connect username edit to controller
    connect(usernameEdit, &QLineEdit::editingFinished, [this, color, usernameEdit]() {
        QString username = usernameEdit->text();
        this->controller->updatePlayerUsername(color, username);
        usernameEdit->clearFocus();
    });

    // Score with increment and decrement buttons
    QHBoxLayout* scoreLayout = new QHBoxLayout;
    QPushButton* decrementButton = new QPushButton("-", this);
    QLabel* scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->setObjectName("scoreLabel");
    QPushButton* incrementButton = new QPushButton("+", this);
    scoreLayout->addWidget(decrementButton);
    scoreLayout->addWidget(scoreLabel);
    scoreLayout->addWidget(incrementButton);
    vbox->addLayout(scoreLayout);

    // Connect score buttons to controller
    connect(incrementButton, &QPushButton::clicked, [this, color]() {
        this->controller->incrementPlayerScore(color);
    });
    connect(decrementButton, &QPushButton::clicked, [this, color]() {
        this->controller->decrementPlayerScore(color);
    });

    // Player stats
    QLabel* statsLabel = new QLabel("Wins: 0\nGames Played: 0\nAvg Score: 0.0", this);
    statsLabel->setObjectName("statsLabel");
    vbox->addWidget(statsLabel);

    groupBox->setLayout(vbox);
    return groupBox;
}