//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_PLAYERPIECE_H
#define AR_SETTLERS_PLAYERPIECE_H
#pragma once
#include "../GamePiece.h"

class PlayerPiece : public GamePiece {
public:
    PlayerPiece(int markerID, const std::string& color) : GamePiece(markerID), color(color) {}

    std::string getColor() const {
        return color;
    }

protected:
    std::string color; // red, blue, orange, white
    cv::Scalar getColorScalar() const {
        if (color == "red") return cv::Scalar(0,0,255);
        if (color == "blue") return cv::Scalar(255,0,0);
        if (color == "orange") return cv::Scalar(0,160,255);
        return cv::Scalar(255,255,255);
    }
};
#endif //AR_SETTLERS_PLAYERPIECE_H
