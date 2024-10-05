//
// Created by Raphael Russo on 9/22/24.
// GUI / View class implementation
//

#include "GUIView.h"
#include <QInputDialog>
#include <QMessageBox>


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
    QPushButton* networkButton = new QPushButton("Enter URL for Stream", this);
    videoButtonsLayout->addWidget(cameraButton);
    videoButtonsLayout->addWidget(networkButton);
    mainLayout->addLayout(videoButtonsLayout);

    // Connect buttons to controller slots for video
    connect(cameraButton, &QPushButton::clicked, controller, &Controller::switchToCamera);
    connect(networkButton, &QPushButton::clicked, [this]() {
        QString netURL = QInputDialog::getText(this, "Enter URL", "URL:");
        if (!netURL.isEmpty()) {
            this->controller->switchToURL(netURL);
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
    connect(controller, &Controller::displayError, this, &GUIView::displayError);

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

        // Update Username if present
        if (usernameEdit && playerInfo.contains("username")) {
            usernameEdit->setText(playerInfo["username"].toString());
        }

        // Update Score if present
        if (scoreLabel && playerInfo.contains("score")) {
            scoreLabel->setText(QString("Score: %1").arg(playerInfo["score"].toInt()));
        }

        // Update Stats selectively
        if (statsLabel) {
            // Fetch current stats from the label
            QString currentStats = statsLabel->text();
            QString currentWins = "Wins: 0";
            QString currentGamesPlayed = "Games Played: 0";
            QString currentAvgScore = "Avg Score: 0.00";

            // Existing stats
            QStringList lines = currentStats.split('\n');
            for (const QString& line : lines) {
                if (line.startsWith("Wins: ")) {
                    currentWins = line;
                } else if (line.startsWith("Games Played: ")) {
                    currentGamesPlayed = line;
                } else if (line.startsWith("Avg Score: ")) {
                    currentAvgScore = line;
                }
            }

            // Update only if new values are provided
            if (playerInfo.contains("wins")) {
                currentWins = QString("Wins: %1").arg(playerInfo["wins"].toInt());
            }
            if (playerInfo.contains("gamesPlayed")) {
                currentGamesPlayed = QString("Games Played: %1").arg(playerInfo["gamesPlayed"].toInt());
            }
            if (playerInfo.contains("averageScore")) {
                currentAvgScore = QString("Avg Score: %1").arg(playerInfo["averageScore"].toDouble(), 0, 'f', 2);
            }

            // Reconstruct stats string
            QString updatedStats = QString("%1\n%2\n%3")
                    .arg(currentWins)
                    .arg(currentGamesPlayed)
                    .arg(currentAvgScore);
            statsLabel->setText(updatedStats);
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

void GUIView::displayError(const QString &message) {
    QMessageBox::warning(this, "Error", message);
}

void GUIView::closeEvent(QCloseEvent* event) {

    controller->endProcessing();
    QWidget::closeEvent(event);  // Proceed with the normal close event
}