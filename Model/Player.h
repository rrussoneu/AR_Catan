//
// Created by Raphael Russo on 9/23/24.
//

#ifndef AR_SETTLERS_PLAYER_H
#define AR_SETTLERS_PLAYER_H

#pragma once
#include <string>


class Player {
public:
    Player(const std::string &color);

    // Getters and Setters

    // Username
    std::string getUsername() const;
    void setUsername(const std::string &username);

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
    std::string getColor() const;

private:
    std::string username;
    int wins;
    int gamesPlayed;
    int score;
    int totalScore;
    std::string color; // Come back to color decision




};


#endif //AR_SETTLERS_PLAYER_H
