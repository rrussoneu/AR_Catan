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
#include "ARObject.h"

class ARObjectFactory {
public:
    using FactoryFunction = std::function<ARObject*(int)>;

    // Register factory function for specific IDs
    static void registerFactory(int markerID, FactoryFunction func) {
        getFactoryMap()[markerID] = func;
    }

    // Create ARObject based on ID
    static ARObject* createARObject(int markerID) {
        auto it = getFactoryMap().find(markerID);
        if (it != getFactoryMap().end()) {
            return it->second(markerID);  // Call the corresponding factory function
        }
        return nullptr;  // Return null if no factory is found for ID
    }

private:
    // Returns a reference to the static factory map
    static std::unordered_map<int, FactoryFunction>& getFactoryMap() {
        static std::unordered_map<int, FactoryFunction> factoryMap;
        return factoryMap;
    }
};
#endif //AR_SETTLERS_AROBJECTFACTORY_H
