//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ROAD_H
#define AR_SETTLERS_ROAD_H
#pragma once
#include "PlayerPiece.h"
#include "../../../ARObjectFactory.h"


class Road : public PlayerPiece {
public:
    Road(int markerID, const std::string &color) : PlayerPiece(markerID, color) {
        //std::vector<cv::Point3f> road_points = {cv::Point3f(-0.02, 0.0, 0.01), cv::Point3f(.02, 0.0, 0.01)};
        std::vector<cv::Point3f> road_points = {cv::Point3f(-0.5, 0.0, 0.5), cv::Point3f(.5, 0.0, 0.5)};

        polygons.push_back(road_points);
        scalePoints(&polygons);

        colors.push_back(getColorScalar());
        //scalePoints();
    }

    static ArucoObject* create(int markerID) {
        if (markerID >= 1 && markerID <= 15) return new Road(markerID, "blue");
        if (markerID >= 16 && markerID <= 30) return new Road(markerID, "red");
        if (markerID >= 31 && markerID <= 45) return new Road(markerID, "orange");
        if (markerID >= 46 && markerID <= 60) return new Road(markerID, "white");
        return nullptr;
    }

    static void registerRoad() {
        for (int markerID = 1; markerID <= 60; ++markerID) {
            ARObjectFactory::registerFactory(markerID, Road::create);
        }
    }

    const std::vector<std::vector<cv::Point3f>>& getPolygons() const override {
        // Return empty or default value
        return polygons;
    }

    const std::vector<cv::Scalar>& getColors() const override {
        // Return empty or default value
        return colors;
    }

private:
    std::vector<std::vector<cv::Point3f>> polygons;
    std::vector<cv::Scalar> colors;
    //void render(cv::Mat &frame, const std::vector<std::vector<cv::Point2f>> &markerCorners) {}
};
#endif //AR_SETTLERS_ROAD_H
