//
// Created by Raphael Russo on 9/25/24.
//

#ifndef AR_SETTLERS_MODELLOADER_H
#define AR_SETTLERS_MODELLOADER_H

#pragma once

#include <string>
#include <vector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLBuffer>
#include "ModelData.h"


class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();

    bool loadModel(const std::string &name, const std::string &filePath);
    ModelData* getModel(const std::string &name);

private:
    std::unordered_map<std::string, ModelData*> models;
};


#endif //AR_SETTLERS_MODELLOADER_H
