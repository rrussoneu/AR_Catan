//
// Created by Raphael Russo on 9/22/24.
//

#include <QApplication>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>
#include "View/GUIView.h"
#include "Model/GameModel.h"
#include "Controller/Controller.h"
#include "Model/InitARObjects.h"
#include "Model/Database/DatabaseManager.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Handle DB directory
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataPath.isEmpty()) {
        QMessageBox::critical(nullptr, "Error", "Unable to determine writable location for DB");
        return -1;
    }

    QDir dir;
    if (!dir.exists(dataPath)) {
        if (!dir.mkpath(dataPath)) {
            QMessageBox::critical(nullptr, "Error", "Failed to create data directory for DB");
            return -1;
        }
    }

    // Define the database file path
    QString dbPath = dataPath + "/game_database.db";

    // Initialize DatabaseManager
    DatabaseManager& dbManager = DatabaseManager::getInstance();
    if (!dbManager.openDatabase(dbPath)) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to open or create DB - EXITING");
        return -1;
    }



    GameModel model;
    initARObjects(model);

    Controller controller(&model);

    GUIView guiView(&controller);
    guiView.show();

    int result = app.exec();
    dbManager.closeDatabase();

    return result;
}
