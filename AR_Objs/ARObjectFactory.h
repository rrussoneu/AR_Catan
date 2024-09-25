//
// Created by Raphael Russo on 9/22/24.
// Factory for making AR Objects
// Good reference: https://www.oodesign.com/factory-pattern
//

#ifndef AR_SETTLERS_AROBJECTFACTORY_H
#define AR_SETTLERS_AROBJECTFACTORY_H
#pragma once
#include <unordered_map>
#include <functional>
#include "Aruco/ArucoObject.h"

class ARObjectFactory {
public:
    using FactoryFunction = std::function<ArucoObject*(int)>;

    static void registerFactory(int markerID, FactoryFunction func) {
        getFactoryMap()[markerID] = func;
    }

    static ArucoObject* createARObject(int markerID) {
        auto it = getFactoryMap().find(markerID);
        if (it != getFactoryMap().end()) {
            return it->second(markerID);
        }
        return nullptr;
    }

private:
    static std::unordered_map<int, FactoryFunction>& getFactoryMap() {
        static std::unordered_map<int, FactoryFunction> factoryMap;
        return factoryMap;
    }
};
#endif //AR_SETTLERS_AROBJECTFACTORY_H
