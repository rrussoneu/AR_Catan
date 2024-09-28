//
// Created by Raphael Russo on 9/23/24.
//

#ifndef AR_SETTLERS_PLAYER_H
#define AR_SETTLERS_PLAYER_H

#pragma once
#include <string>
#include <QString>


class Player {
public:
    Player();

    // Getters and Setters

    // Username
    QString getUsername() const;
    void setUsername(const QString &username);

    // Wins
    int getWins() const;
    void setWins(int wins);

    // Games
    int getGamesPlayed() const;
    void setGamesPlayed(int gamesPlayed);

    // Score
    int getScore() const;
    void setScore(int score);
    void incrementScore();
    void decrementScore();
    float getAverageScore() const;
    int getTotalScore() const;
    void setTotalScore(int total);


    // Color - maybe remove this at some point idk if necessary or make an enum
    // Can just do a class for color w enum and scalar value in case it is needed multiple places, would be better design
    //std::string getColor() const;

private:
    QString username;
    int wins;
    int gamesPlayed;
    int score;
    int totalScore;





};


#endif //AR_SETTLERS_PLAYER_H
