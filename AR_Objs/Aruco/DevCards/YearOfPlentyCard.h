//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_YEAROFPLENTYCARD_H
#define AR_SETTLERS_YEAROFPLENTYCARD_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class YearOfPlentyCard : public DevelopmentCard {
public:
    YearOfPlentyCard(int markerID) : DevelopmentCard(markerID, &OpenCVRenderStrategy::getInstance()) {
        std::vector<cv::Point3f> vertical = {cv::Point3f(0.0f,0.0f,0.25f), cv::Point3f(0.0f,0.0f,0.75f)};
        std::vector<cv::Point3f> horizontal = {cv::Point3f(-0.25f, 0.0, 0.5f), cv::Point3f(0.25f, 0.0, 0.5f)};
        polygons.push_back(vertical);
        polygons.push_back(horizontal);
        scalePoints(&polygons);
        colors.push_back(cv::Scalar(0,255,0));
        colors.push_back(cv::Scalar(0,255,0));
    }



    static ArucoObject* create(int markerID) {
        return new YearOfPlentyCard(markerID);
    }

    static void registerYearOfPlentyCard() {
        for (int markerID = 116; markerID <= 117; ++markerID) {
            ARObjectFactory::registerFactory(markerID, YearOfPlentyCard::create);
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
#endif //AR_SETTLERS_YEAROFPLENTYCARD_H
