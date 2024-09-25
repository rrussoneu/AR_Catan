//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_MONOPOLYCARD_H
#define AR_SETTLERS_MONOPOLYCARD_H
#pragma once
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"
#include "../../Rendering/OpenCVRenderStrategy.h"

// Add later: public HomographyObj
class MonopolyCard : public DevelopmentCard {
public:
    MonopolyCard(int markerID) : DevelopmentCard(markerID, &OpenCVRenderStrategy::getInstance()) {


        std::vector<cv::Point3f> sBottomCurve = {
                cv::Point3f(-0.5f,  0.0f, 0.1f),
                cv::Point3f( 0.25f, 0.0f, 0.1f),
                cv::Point3f( 0.5f,  0.0f, 0.2f),
                cv::Point3f( 0.25f, 0.0f, 0.3f)
        };


        std::vector<cv::Point3f> sTopCurve = {
                cv::Point3f(-0.25f, 0.0f, 0.4f),
                cv::Point3f(-0.5f,  0.0f, 0.55f),
                cv::Point3f(-0.25f, 0.0f, 0.7f),
                cv::Point3f( 0.5f,  0.0f, 0.7f),
                cv::Point3f(-0.25f, 0.0f, 0.9f)
        };


        std::vector<cv::Point3f> leftVerical = {
                cv::Point3f(-0.25f, 0.0f, 0.0f),
                cv::Point3f(-0.25f, 0.0f, 1.0f)
        };


        std::vector<cv::Point3f> rightVertical = {
                cv::Point3f( 0.25f, 0.0f, 0.0f),
                cv::Point3f( 0.25f, 0.0f, 1.0f)
        };


        polygons.push_back(sTopCurve);
        polygons.push_back(sBottomCurve);
        polygons.push_back(leftVerical);
        polygons.push_back(rightVertical);

        scalePoints(&polygons);

        for (int i = 0; i < polygons.size(); ++i) {
            colors.push_back(cv::Scalar(0,255,0));
        }

    }


    static ArucoObject* create(int markerID) {
        return new MonopolyCard(markerID);
    }

    static void registerMonopolyCard() {
        for (int markerID = 114; markerID <= 115; ++markerID) {
            ARObjectFactory::registerFactory(markerID, MonopolyCard::create);
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
#endif //AR_SETTLERS_MONOPOLYCARD_H
