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
#include "../../Model/MessageEmitter.h"

/**
 * Class for loading model data from .obj files
 */
class ModelLoader: public MessageEmitter {
public:
    explicit ModelLoader(QObject *parent = nullptr);
    ~ModelLoader();

    /**
     * Load model from file path and put in map of models and their data
     * @param name The name of the model
     * @param filePath The file path
     * @return True on success
     */
    bool loadModel(const std::string &name, const std::string &filePath);


    /**
     * Gets a model's data
     * @param name Name of the model to get
     * @return The data for the model
     */
    std::shared_ptr<ModelData> getModel(const std::string &name);

private:
    std::unordered_map<std::string, std::shared_ptr<ModelData>> models;
};


#endif //AR_SETTLERS_MODELLOADER_H
