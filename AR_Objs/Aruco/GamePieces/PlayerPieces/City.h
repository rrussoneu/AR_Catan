//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_CITY_H
#define AR_SETTLERS_CITY_H
#pragma once
#include "PlayerPiece.h"
#include "../../../ARObjectFactory.h"


class City : public PlayerPiece {
public:
    City(int markerID, const std::string &color) : PlayerPiece(markerID, color) {

        // Vertices for base - similar to settlement in structure but with a little house like structure on top not just a roof
        std::vector<cv::Point3f> base = {
                cv::Point3f(-0.5f, -0.5f, 0.0f), // Bottom left - front
                cv::Point3f( 0.5f, -0.5f, 0.0f), // Bottom right - front
                cv::Point3f( 0.5f,  0.5f, 0.0f), // Top right - back
                cv::Point3f(-0.5f,  0.5f, 0.0f)  // Top left - back
        };

        std::vector<cv::Point3f> top = {
                cv::Point3f(-0.5f, -0.5f, 1.0f), // Top left - front
                cv::Point3f( 0.5f, -0.5f, 1.0f), // Top right - front
                cv::Point3f( 0.5f,  0.5f, 1.0f), // Top right - back
                cv::Point3f(-0.5f,  0.5f, 1.0f)  // Top left - back
        };

        // Vertices for top house
        std::vector<cv::Point3f> topHouseBase = {
                cv::Point3f(-0.25f, -0.5f, 1.0f), // Bottom left - front
                cv::Point3f( 0.25f, -0.5f, 1.0f), // Bottom right - front
                cv::Point3f( 0.25f,  0.0f, 1.0f), // Top right - back
                cv::Point3f(-0.25f,  0.0f, 1.0f)  // Top left - back
        };

        std::vector<cv::Point3f> topHouseTop = {
                cv::Point3f(-0.25f, -0.5f, 1.5f), // Top left - front
                cv::Point3f( 0.25f, -0.5f, 1.5f), // Top right - front
                cv::Point3f( 0.25f,  0.0f, 1.5f), // Top right - back
                cv::Point3f(-0.25f,  0.0f, 1.5f)  // Top left - back
        };

        // Roof peak
        cv::Point3f roof_peak(0.0f, -0.5f, 2.0f);  // Peak above front face of top house

        // Define polygons

        // Bottom
        polygons.push_back({ base[0], base[1], base[2], base[3] });

        // Sides
        polygons.push_back({ base[0], base[1], top[1], top[0] }); // Front
        polygons.push_back({ base[1], base[2], top[2], top[1] }); // Right
        polygons.push_back({ base[2], base[3], top[3], top[2] }); // Back
        polygons.push_back({ base[3], base[0], top[0], top[3] }); // Left

        // Top
        polygons.push_back({ topHouseBase[0], topHouseBase[1], topHouseTop[1], topHouseTop[0] }); // Front
        polygons.push_back({ topHouseBase[1], topHouseBase[2], topHouseTop[2], topHouseTop[1] }); // Right
        polygons.push_back({ topHouseBase[2], topHouseBase[3], topHouseTop[3], topHouseTop[2] }); // Back
        polygons.push_back({ topHouseBase[3], topHouseBase[0], topHouseTop[0], topHouseTop[3] }); // Left

        // Top part's roof
        polygons.push_back({ topHouseTop[0], roof_peak, topHouseTop[1] });  // Front
        polygons.push_back({ topHouseTop[3], roof_peak, topHouseTop[2] });  // Back

        scalePoints(&polygons);
        for (int i = 0; i < polygons.size(); ++i) {
            colors.push_back(getColorScalar());
        }

    }

    static ArucoObject* create(int markerID) {
        if (markerID >= 81 && markerID <= 84) return new City(markerID, "blue");
        if (markerID >= 85 && markerID <= 88) return new City(markerID, "red");
        if (markerID >= 89 && markerID <= 92) return new City(markerID, "orange");
        if (markerID >= 93 && markerID <= 96) return new City(markerID, "white");
        return nullptr;
    }

    static void registerCity() {
        for (int markerID = 81; markerID <= 96; ++markerID) {
            ARObjectFactory::registerFactory(markerID, City::create);
        }
    }



    const std::vector<std::vector<cv::Point3f>>& getPolygons() const override {
        return polygons;
    }

    const std::vector<cv::Scalar>& getColors() const override {
        return colors;
    }
private:
    std::vector<std::vector<cv::Point3f>> polygons;
    std::vector<cv::Scalar> colors;
};
#endif //AR_SETTLERS_CITY_H
