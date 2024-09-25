//
// Created by Raphael Russo on 9/24/24.
//

#ifndef AR_SETTLERS_PLAYERINFOBUILDER_H
#define AR_SETTLERS_PLAYERINFOBUILDER_H
#pragma once
#include <QString>
#include <QVariant>
#include <QMap>
#include <optional>

class PlayerInfoBuilder {
public:
    PlayerInfoBuilder& setColor(const QString &value) {
        color = value;
        return *this;
    }
    PlayerInfoBuilder& setUsername(const QString &value) {
        username = value;
        return *this;
    }
    PlayerInfoBuilder& setScore(int value) {
        score = value;
        return *this;
    }
    PlayerInfoBuilder& setWins(int value) {
        wins = value;
        return *this;
    }
    PlayerInfoBuilder& setGamesPlayed(int value) {
        gamesPlayed = value;
        return *this;
    }
    PlayerInfoBuilder& setAverageScore(double value) {
        averageScore = value;
        return *this;
    }

    QMap<QString, QVariant> build() const {
        QMap<QString, QVariant> info;
        if (color) info["color"] = *color;
        if (username) info["username"] = *username;
        if (score) info["score"] = *score;
        if (wins) info["wins"] = *wins;
        if (gamesPlayed) info["gamesPlayed"] = *gamesPlayed;
        if (averageScore) info["averageScore"] = *averageScore;
        return info;
    }

private:
    std::optional<QString> color;
    std::optional<QString> username;
    std::optional<int> score;
    std::optional<int> wins;
    std::optional<int> gamesPlayed;
    std::optional<double> averageScore;
};
#endif //AR_SETTLERS_PLAYERINFOBUILDER_H
