//
// Created by Raphael Russo on 9/25/24.
//

#ifndef AR_SETTLERS_OPENGLRENDERSTRATEGY_H
#define AR_SETTLERS_OPENGLRENDERSTRATEGY_H

#pragma once

#include "RenderStrategy.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include "ModelLoader.h"
#include "../../Config.h"
#include <QtGui/qimage.h>
#include <QtCore/qdir.h>

class OpenGLRenderStrategy : public RenderStrategy, protected QOpenGLFunctions {
public:
    OpenGLRenderStrategy();
    ~OpenGLRenderStrategy();

    int getType() override {
        return 2;
    }

    void prepareForRendering(const cv::Mat &frame);

    QMatrix4x4 createProjectionMatrix(const cv::Mat &cameraMatrix, int width, int height, double nearPlane, double farPlane);

    QMatrix4x4 createModelViewMatrix(const cv::Vec3d &rvec, const cv::Vec3d &tvec);

    bool initialize(int frameWidth, int frameHeight) override;

    void finalizeRendering(cv::Mat &frame);

    void render(ARObject *object, cv::Mat &frame,
                const std::vector<cv::Point2f> &markerCorners,
                const cv::Vec3d &rvec, const cv::Vec3d &tvec,
                const std::vector<double> &distCoeffs,
                const cv::Mat &cameraMatrix) override;

    bool loadModels(const std::vector<std::string> &modelNames);


private:
    //QOpenGLContext *glContext; - Owned by the processing thread with one instance of this strategy to only have one context exist
    QOpenGLShaderProgram shaderProgram;
    QOpenGLFramebufferObject *fbo;
    ModelLoader modelLoader;
    std::unordered_map<std::string, ModelData*> models;

};

#endif //AR_SETTLERS_OPENGLRENDERSTRATEGY_H
