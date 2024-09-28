//
// Created by Raphael Russo on 9/25/24.
//

// OpenGLRenderStrategy.cpp
#include "OpenGLRenderStrategy.h"
#include "../../Config.h"
#include <QDebug>


OpenGLRenderStrategy::OpenGLRenderStrategy(QOpenGLContext *context)
        : glContext(context), fbo(nullptr) {
    /* Init called in process thread when OpenGLRenderStrategy is first needed.
     * Only one instance should exist to handle threading/ context.
     * All objects needing this strat get the pointer for it
     * */
}

OpenGLRenderStrategy::~OpenGLRenderStrategy() {
    if (fbo) {
        delete fbo;
    }
    //for (auto& pair : models) {
    //    delete pair.second;
    //}
}


void OpenGLRenderStrategy::render(ARObject *object, cv::Mat &frame,
                                  const std::vector<cv::Point2f> &markerCorners,
                                  const cv::Vec3d &rvec, const cv::Vec3d &tvec,
                                  const std::vector<double> &distCoeffs,
                                  const cv::Mat &cameraMatrix) {


    // Get the model name associated with the marker/ AR object
    std::string modelName = object->getName();

    // Get data for object model
    //ModelData *modelData = modelLoader.getModel(modelName);
    ModelData *modelData = models.at(modelName);
    if (!modelData) {
        qWarning() << "Model not found:" << QString::fromStdString(modelName);
        return;
    }

    // Set up projection and view matrices
    QMatrix4x4 projectionMatrix = createProjectionMatrix(cameraMatrix, frame.cols, frame.rows, 0.1, 100.0);
    QMatrix4x4 modelViewMatrix = createModelViewMatrix(rvec, tvec);

    // Apply scaling based on size of marker
    float scale = Config::getInstance().getMarkerLength();
    modelViewMatrix.scale(scale, scale, scale);

    // Bind shader program and set uniforms
    shaderProgram.bind();
    shaderProgram.setUniformValue("uProjectionMatrix", projectionMatrix);
    shaderProgram.setUniformValue("uModelViewMatrix", modelViewMatrix);

    // Bind the VAO from the model
    modelData->vao.bind();

    // Draw the model
    glDrawArrays(GL_TRIANGLES, 0, modelData->vertexCount);

    // Unbind VAO and shader program
    modelData->vao.release();
    shaderProgram.release();
}



bool OpenGLRenderStrategy::loadModels(const std::vector<std::string>& modelNames) {
    for (const auto &name : modelNames) {
        std::string filePath = "AR_Objs/Rendering/Models/" + name + ".obj"; // Adjust the path as needed
        if (!modelLoader.loadModel(name, filePath)) {
            qWarning() << "Failed to load model:" << QString::fromStdString(name);
            return false;
        }
        models[name] = modelLoader.getModel(name);
    }
    return true;
}
