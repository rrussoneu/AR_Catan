//
// Created by Raphael Russo on 9/24/24.
//

#ifndef AR_SETTLERS_UPDATEUSERNAMECOMMAND_H
#define AR_SETTLERS_UPDATEUSERNAMECOMMAND_H
#pragma once
#include "Command.h"
#include "../../Model/GameModel.h"
#include "../PlayerInfoBuilder.h"
#include "../../Model/Database/PlayerService.h"

class UpdateUsernameCommand : public Command {
private:
    GameModel *model;
    QString color;
    QString username;

public:
    UpdateUsernameCommand(GameModel *model, const QString &color, const QString &username)
            : model(model), color(color), username(username) {}

    QMap<QString, QVariant> run() override {
        Player *player = model->getPlayer(color);
        if (player) {
            player->setUsername(username);
            PlayerService playerService;
            if (!playerService.fetchOrCreatePlayer(*player)) {
                qWarning("Failed to fetch or create player!!!");

            }

            return PlayerInfoBuilder()
                    .setColor(color)
                    .setUsername(username)
                    .setAverageScore(player->getAverageScore())
                    .setScore(player->getScore())
                    .setWins(player->getWins())
                    .setGamesPlayed(player->getGamesPlayed())
                    .build();
        }
        return {};
    }
};

#endif //AR_SETTLERS_UPDATEUSERNAMECOMMAND_H
