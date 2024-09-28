//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_ORERESCARD_H
#define AR_SETTLERS_ORERESCARD_H
#pragma once
#include "ResourceCard.h"
#include "../../ARObjectFactory.h"
#include "../../Rendering/OpenGLRenderStrategy.h"

class OreResCard : public ResourceCard {
public:
    OreResCard(int markerID, RenderStrategy *renderStrategy = nullptr, const std::string &objectName = "ore") : ResourceCard(markerID, renderStrategy, objectName) {}



    static ArucoObject* create(int markerID) {
        return new OreResCard(markerID);
    }

    static void registerOreResCard() {
        ARObjectFactory::registerFactory(127, OreResCard::create);

    }

    const std::vector<std::vector<cv::Point3f>>& getPolygons() const override {
        // Return empty or default value
        static std::vector<std::vector<cv::Point3f>> emptyPolygons;
        return emptyPolygons;
    }

    const std::vector<cv::Scalar>& getColors() const override {
        // Return empty or default value
        static std::vector<cv::Scalar> emptyColors;
        return emptyColors;
    }

    std::string getModelName() {
        return "Ore";
    }

};
#endif //AR_SETTLERS_ORERESCARD_H
