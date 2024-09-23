//
// Created by Raphael Russo on 9/22/24.
//

#include <QApplication>
#include "View/GUIView.h"
#include "Model/GameModel.h"
#include "Controller/Controller.h"
#include "Model/InitARObjects.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GameModel model;
    initARObjects(model);

    Controller controller(&model);

    GUIView guiView(&controller);
    guiView.show();

    return app.exec();
}
