//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_VICTORYPOINTCARD_H
#define AR_SETTLERS_VICTORYPOINTCARD_H
#include "DevelopmentCard.h"
#include "../../ARObjectFactory.h"

// Add later: public HomographyObj
class VictoryPointCard : public DevelopmentCard {
public:
    VictoryPointCard(int markerID) : DevelopmentCard(markerID, &OpenCVRenderStrategy::getInstance()) {
        std::vector<cv::Point3f> frontMain = {cv::Point3f(-0.25f, 0.5f, 0.0f), cv::Point3f(-0.25f, 0.5f, 0.5f), cv::Point3f(0.25f, 0.5f, 0.5f), cv::Point3f(0.25f, 0.5f, 0.0f)};
        std::vector<cv::Point3f> frontLeft = {cv::Point3f(-0.25f, 0.5f, 0.0f), cv::Point3f(-0.25f, 0.5f, 0.25f), cv::Point3f(-0.5f, 0.25f, 0.25f), cv::Point3f(-0.5f, 0.25f, 0.0f)};
        std::vector<cv::Point3f> frontRight = {cv::Point3f(0.25f, 0.5f, 0.0f), cv::Point3f(0.25f, 0.5f, 0.25f), cv::Point3f(0.5f, 0.25f, 0.25f), cv::Point3f(0.5f, 0.25f, 0.0f)};
        std::vector<cv::Point3f> leftMain = {cv::Point3f(-0.5f, 0.25f, 0.0f), cv::Point3f(-0.5f, 0.25f, 0.5f), cv::Point3f(-0.5f, -0.25f, 0.5f), cv::Point3f(-0.5f, -0.25f, 0.0f)};
        std::vector<cv::Point3f> rightMain = {cv::Point3f(0.5f, 0.25f, 0.0f), cv::Point3f(0.5f, 0.25f, 0.5f), cv::Point3f(0.5f, -0.25f, 0.5f), cv::Point3f(0.5f, -0.25f, 0.0f)};
        std::vector<cv::Point3f> backLeft = {cv::Point3f(-0.25f, -0.5f, 0.0f), cv::Point3f(-0.25f, -0.5f, 0.25f), cv::Point3f(-0.5f, -0.25f, 0.25f), cv::Point3f(-0.5f, -0.25f, 0.0f)};
        std::vector<cv::Point3f> backRight = {cv::Point3f(0.25f, -0.5f, 0.0f), cv::Point3f(0.25f, -0.5f, 0.25f), cv::Point3f(0.5f, -0.25f, 0.25f), cv::Point3f(0.5f, -0.25f, 0.0f)};
        std::vector<cv::Point3f> backMain = {cv::Point3f(-0.25f, -0.5f, 0.0f), cv::Point3f(-0.25f, -0.5f, 0.5f), cv::Point3f(0.25f, -0.5f, 0.5f), cv::Point3f(0.25f, -0.5f, 0.0f)};

        polygons.push_back(frontMain);
        polygons.push_back(frontLeft);
        polygons.push_back(frontRight);
        polygons.push_back(leftMain);
        polygons.push_back(rightMain);
        polygons.push_back(backLeft);
        polygons.push_back(backRight);
        polygons.push_back(backMain);

        scalePoints(&polygons);
        for (int i = 0; i < polygons.size(); ++i) {
            colors.push_back(cv::Scalar(50,170,210));
        }
    }



    static ArucoObject* create(int markerID) {
        return new VictoryPointCard(markerID);
    }

    static void registerVictoryPointCard() {
        for (int markerID = 120; markerID <= 124; ++markerID) {
            ARObjectFactory::registerFactory(markerID, VictoryPointCard::create);
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
#endif //AR_SETTLERS_VICTORYPOINTCARD_H
