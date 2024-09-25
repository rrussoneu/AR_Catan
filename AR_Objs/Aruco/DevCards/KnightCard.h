//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_KNIGHTCARD_H
#define AR_SETTLERS_KNIGHTCARD_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"
#include "../../Rendering/OpenCVRenderStrategy.h"


class KnightCard : public DevelopmentCard {
public:
    KnightCard(int markerID): DevelopmentCard(markerID, &OpenCVRenderStrategy::getInstance()) {

        std::vector<cv::Point3f> sword = {cv::Point3f(0.0f, 0.0f, 0.0f), cv::Point3f(0.0f,0.0f, 0.65f)};
        std::vector<cv::Point3f> guard = {cv::Point3f(-0.15f, 0.0f, 0.65f), cv::Point3f(0.15f,0.0f, 0.65f)};
        std::vector<cv::Point3f> handle = {cv::Point3f(0.0f, 0.0f, 0.65f), cv::Point3f(0.0f,0.0f, 0.8f)};
        polygons.push_back(sword);
        polygons.push_back(guard);
        polygons.push_back(handle);
        scalePoints(&polygons);
        colors.push_back(cv::Scalar(80,80,80));
        colors.push_back(cv::Scalar(20,70,140));
        colors.push_back(cv::Scalar(20,70,140));
    }

    static void registerKnightCard() {
        for (int markerID = 100; markerID <= 113; ++markerID) {
            ARObjectFactory::registerFactory(markerID, KnightCard::create);
        }
    }

    static ArucoObject* create(int markerID) {
        return new KnightCard(markerID);
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
};


#endif //AR_SETTLERS_KNIGHTCARD_H
