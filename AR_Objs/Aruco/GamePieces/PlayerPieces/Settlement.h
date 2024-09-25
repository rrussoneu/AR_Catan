//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_SETTLEMENT_H
#define AR_SETTLERS_SETTLEMENT_H
#pragma once
#include "PlayerPiece.h"
#include "../../../ARObjectFactory.h"


class Settlement : public PlayerPiece {
public:
    Settlement(int markerID, const std::string &color) : PlayerPiece(markerID, color) {

        // Vertices rectangular settlement
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

        // Roof peak
        cv::Point3f roof_peak_front(0.0f, -0.5f, 1.5f);  // Peak above front
        cv::Point3f roof_peak_back(0.0f,  0.5f, 1.5f);   // Peak above back

        // Polygons


        // Bottom
        polygons.push_back({ base[0], base[1], base[2], base[3] });

        // Sides
        polygons.push_back({ base[0], base[1], top[1], top[0] }); // Front
        polygons.push_back({ base[1], base[2], top[2], top[1] }); // Right
        polygons.push_back({ base[2], base[3], top[3], top[2] }); // Back
        polygons.push_back({ base[3], base[0], top[0], top[3] }); // Left

        // Roof
        polygons.push_back({ top[0], roof_peak_front, top[1] });  // Front
        polygons.push_back({ top[3], roof_peak_back, top[2] });   // Back
        polygons.push_back({ top[0], roof_peak_front, roof_peak_back, top[3] }); // Left slope
        polygons.push_back({ top[1], roof_peak_front, roof_peak_back, top[2] }); // Right slope

        // Scale them to the marker size
        scalePoints(&polygons);


        for (int i = 0; i < polygons.size(); ++i) {
            colors.push_back(getColorScalar());
        }

    }

    static ArucoObject* create(int markerID) {
        if (markerID >= 61 && markerID <= 65) return new Settlement(markerID, "blue");
        if (markerID >= 66 && markerID <= 70) return new Settlement(markerID, "red");
        if (markerID >= 71 && markerID <= 75) return new Settlement(markerID, "orange");
        if (markerID >= 76 && markerID <= 80) return new Settlement(markerID, "white");
        return nullptr;
    }

    static void registerSettlement() {
        for (int markerID = 61; markerID <= 80; ++markerID) {
            ARObjectFactory::registerFactory(markerID, Settlement::create);
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
#endif //AR_SETTLERS_SETTLEMENT_H
