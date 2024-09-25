//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ROADBUILDING_H
#define AR_SETTLERS_ROADBUILDING_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class RoadBuildingCard : public DevelopmentCard {
public:
    RoadBuildingCard(int markerID) : DevelopmentCard(markerID, &OpenCVRenderStrategy::getInstance()) {
        std::vector<cv::Point3f> metalBottom = {
                cv::Point3f(0.0f, 0.0f, 0.0f),          // Tip
                cv::Point3f(-0.25f, 0.0f, 0.2f),      // Bottom left
                cv::Point3f(-0.25f, 0.0f, 0.45f),       // Bottom right
                cv::Point3f(0.25f, 0.0f, 0.45f),          // Top left
                cv::Point3f(0.25f, 0.0f, 0.2f),           // Top right
        };
        std::vector<cv::Point3f> handleVertical {
                cv::Point3f(0.0f, 0.0f, 0.45f),           // Start of handle
                cv::Point3f(0.0f, 0.0f, 0.8f),           // Top of handle
        };
        std::vector<cv::Point3f> handleHorizontal {
                cv::Point3f(-0.2f, 0.0f, 0.8f),          // Top left handle
                cv::Point3f(0.2f, 0.0f, 0.8f)            // Top right handle
        };
        polygons.push_back(metalBottom);
        polygons.push_back(handleVertical);
        polygons.push_back(handleHorizontal);
        scalePoints(&polygons);
        colors.push_back(cv::Scalar(60,74,178));
        colors.push_back(cv::Scalar(20,70,140));
        colors.push_back(cv::Scalar(20,70,140));

    }

    static ArucoObject* create(int markerID) {
        return new RoadBuildingCard(markerID);
    }

    static void registerRoadBuildingCard() {
        for (int markerID = 118; markerID <= 119; ++markerID) {
            ARObjectFactory::registerFactory(markerID, RoadBuildingCard::create);
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
#endif //AR_SETTLERS_ROADBUILDING_H
