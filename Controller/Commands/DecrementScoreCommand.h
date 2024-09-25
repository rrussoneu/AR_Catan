//
// Created by Raphael Russo on 9/24/24.
//

#ifndef AR_SETTLERS_DECREMENTSCORECOMMAND_H
#define AR_SETTLERS_DECREMENTSCORECOMMAND_H
#pragma once
#include "Command.h"
#include "../../Model/GameModel.h"
#include "../PlayerInfoBuilder.h"

class DecrementScoreCommand : public Command {
private:
    GameModel *model;
    QString color;

public:
    DecrementScoreCommand(GameModel *model, const QString &color)
            : model(model), color(color) {}

    QMap<QString, QVariant> run() override {
        Player *player = model->getPlayer(color.toStdString());
        if (player) {
            player->decrementScore();
            return PlayerInfoBuilder()
                    .setColor(color)
                    .setScore(player->getScore())
                    .build();
        }
        return {};
    }
};
#endif //AR_SETTLERS_DECREMENTSCORECOMMAND_H
