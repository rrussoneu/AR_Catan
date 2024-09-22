//
// Created by Raphael Russo on 9/22/24.
//

#include <QApplication>
#include "View/GUIView.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Initialize the Qt application

    GUIView window;  // Create the main window (includes camera feed and buttons)
    window.resize(800, 600);  // Set the initial size of the window
    window.show();  // Show the window

    return app.exec();  // Start the Qt event loop
}
