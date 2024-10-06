//
// Created by Raphael Russo on 9/22/24.
// GUI / View class implementation
//

#include "GUIView.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QCloseEvent>


GUIView::GUIView(Controller* controller, QWidget* parent)
        : QWidget(parent), controller(controller) {
    // Default font
    QFont defaultFont("Arial", 12);
    QWidget::setFont(defaultFont);

    // Main Grid Layout
    QGridLayout* mainGridLayout = new QGridLayout;

    // Video
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(640, 480);
    videoLabel->setAlignment(Qt::AlignCenter);
    // Add below back in maybe for a movie like border situation
    //videoLabel->setStyleSheet("background-color: black; border: 1px solid #ccc;");
    mainGridLayout->addWidget(videoLabel, 0, 0, 1, 2, Qt::AlignCenter);

    // Video Buttons
    QHBoxLayout* videoButtonsLayout = new QHBoxLayout;
    QPushButton* cameraButton = new QPushButton("Switch to Camera", this);
    cameraButton->setIcon(QIcon("Styles/Icons/camera-solid.svg"));
    QPushButton* networkButton = new QPushButton("Enter URL for Stream", this);
    networkButton->setIcon(QIcon("Styles/Icons/globe-solid.svg"));
    videoButtonsLayout->addWidget(cameraButton);
    videoButtonsLayout->addWidget(networkButton);
    mainGridLayout->addLayout(videoButtonsLayout, 1, 0, 1, 2, Qt::AlignCenter);

    // Connect video buttons to controller
    connect(cameraButton, &QPushButton::clicked, controller, &Controller::switchToCamera);
    connect(networkButton, &QPushButton::clicked, [this]() {
        QString netURL = QInputDialog::getText(this, "Enter URL", "URL:");
        if (!netURL.isEmpty()) {
            this->controller->switchToURL(netURL);
        }
    });

    // Dice rolls
    QHBoxLayout* diceLayout = new QHBoxLayout;
    rollDiceButton = new QPushButton("Roll Dice", this);
    diceRollLabel = new QLabel("Dice Roll: ", this);
    diceRollLabel->setAlignment(Qt::AlignCenter);
    diceRollLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    rollDiceButton->setIcon(QIcon("Styles/Icons/dice-solid.svg"));
    diceLayout->addWidget(rollDiceButton);
    diceLayout->addWidget(diceRollLabel);
    mainGridLayout->addLayout(diceLayout, 2, 0, 1, 2, Qt::AlignCenter);

    connect(rollDiceButton, &QPushButton::clicked, controller, &Controller::rollDice);

    // Finish game
    finishGameButton = new QPushButton("Finish Game", this);
    finishGameButton->setIcon(QIcon("Styles/Icons/flag-checkered-solid.svg"));
    mainGridLayout->addWidget(finishGameButton, 3, 0, 1, 2, Qt::AlignCenter);

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
    mainGridLayout->addLayout(playersLayout, 4, 0, 1, 2);

    // Main layout = the grid layout
    setLayout(mainGridLayout);

    // Add a simple style sheet
    QFile styleFile(":/styles/stylesheet.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
    }

    // Connect signals/slots
    connect(controller, &Controller::frameReady, this, &GUIView::updateVideoFeed);
    connect(controller, &Controller::diceRolled, this, &GUIView::updateDiceRoll);
    connect(controller, &Controller::playerUpdated, this, &GUIView::updatePlayerInfo);
    connect(controller, &Controller::displayError, this, &GUIView::displayError);
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

    QGroupBox* groupBox = new QGroupBox(this);
    QVBoxLayout* vbox = new QVBoxLayout;

    QHBoxLayout* headerLayout = new QHBoxLayout;

    // Icon label
    QLabel* iconLabel = new QLabel(this);
    QString iconPath = QString("Styles/Icons/player-blue.svg"); // Add back .arg(color.toLower()) after figuring out best icons
    QPixmap iconPixmap(iconPath);
    if (!iconPixmap.isNull()) {
        // Scale the icon
        iconLabel->setPixmap(iconPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // Handle missing icon
        iconLabel->setText("👤"); // Use generic
        iconLabel->setFont(QFont("Arial", 16));
    }

    // Title
    QLabel* titleLabel = new QLabel(QString("%1 PLAYER").arg(color.toUpper()), this);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    titleLabel->setFont(titleFont);

    // Add icon and title to the header layout
    headerLayout->addWidget(iconLabel);
    headerLayout->addSpacing(8); // Space between icon and text
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch(); // Push content to the left

    vbox->addLayout(headerLayout);

    // Player color background color
    QString bgColor;
    if (color.toLower() == "blue") bgColor = "#ADD8E6";
    else if (color.toLower() == "red") bgColor = "#F08080";
    else if (color.toLower() == "orange") bgColor = "#FFD580";
    else if (color.toLower() == "white") bgColor = "#FFFFFF";
    else bgColor = "#FFFFFF"; // Default color

    groupBox->setStyleSheet(QString(
            "QGroupBox {"
            "   background-color: %1;"
            "   border: 1px solid gray;"
            "   border-radius: 5px;"
            "   margin-top: 10px;"
            "}"
            "QGroupBox::title {"
            "   subcontrol-origin: margin;"
            "   subcontrol-position: top center;"
            "   padding: 0 3px;"
            "}"
    ).arg(bgColor));

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

    QString buttonStyle =
            "QPushButton {"
            "   background-color: #FF5722;"
            "   color: white;"
            "   padding: 4px 8px;"
            "   border: none;"
            "   border-radius: 3px;"
            "   font-size: 16px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #E64A19;"
            "}";
    decrementButton->setStyleSheet(buttonStyle);
    incrementButton->setStyleSheet(buttonStyle);
    scoreLayout->addWidget(decrementButton);
    scoreLayout->addWidget(scoreLabel, 1, Qt::AlignCenter);
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

// https://stackoverflow.com/questions/17480984/how-do-i-handle-the-event-of-the-user-pressing-the-x-close-button
void GUIView::closeEvent(QCloseEvent *event) {
    // Confirm before closing
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Confirm Exit",
                                                               tr("Are you sure you want to exit?\n"),
                                                               QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        controller->endProcessing();
        event->accept();
    }
}