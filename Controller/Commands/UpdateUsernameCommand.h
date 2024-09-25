//
// Created by Raphael Russo on 9/24/24.
//

#ifndef AR_SETTLERS_UPDATEUSERNAMECOMMAND_H
#define AR_SETTLERS_UPDATEUSERNAMECOMMAND_H
#pragma once
#include "Command.h"
#include "../../Model/GameModel.h"
#include "../PlayerInfoBuilder.h"

class UpdateUsernameCommand : public Command {
private:
    GameModel *model;
    QString color;
    QString username;

public:
    UpdateUsernameCommand(GameModel *model, const QString &color, const QString &username)
            : model(model), color(color), username(username) {}

    QMap<QString, QVariant> run() override {
        Player *player = model->getPlayer(color.toStdString());
        if (player) {
            player->setUsername(username.toStdString());
            return PlayerInfoBuilder()
                    .setColor(color)
                    .setUsername(username)
                    .build();
        }
        return {};
    }
};

#endif //AR_SETTLERS_UPDATEUSERNAMECOMMAND_H
