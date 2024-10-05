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
    Q_OBJECT
public:
    OpenGLRenderStrategy(QObject *parent = nullptr);
    ~OpenGLRenderStrategy();

    // Type 2 for OpenGL strategy
    int getType() override {
        return 2;
    }

    void prepareForRendering(const cv::Mat &frame);

    /**
     * Creates the projection matrix
     * @param cameraMatrix  Camera intrinsic parameters
     * @param width Frame width
     * @param height Frame height
     * @param nearPlane Front clip plane
     * @param farPlane Back clip plane
     * @return The matrix
     */
    QMatrix4x4 createProjectionMatrix(const cv::Mat &cameraMatrix, int width, int height, double nearPlane, double farPlane);

    /**
     * Creats a view matrix for a model based on rotation and translation vectors from pose estimation
     * @param rvec Rotation vectors
     * @param tvec Translation vectors
     * @return The matrix
     */
    QMatrix4x4 createModelViewMatrix(const cv::Vec3d &rvec, const cv::Vec3d &tvec);

    bool initialize(int frameWidth, int frameHeight) override;

    /**
     * Blends the OpenGL rendered offscreen frame with the frame to display
     * @param frame The frame from the video feed to display
     */
    void finalizeRendering(cv::Mat &frame);

    void render(ARObject *object, cv::Mat &frame,
                const std::vector<cv::Point2f> &markerCorners,
                const cv::Vec3d &rvec, const cv::Vec3d &tvec,
                const std::vector<double> &distCoeffs,
                const cv::Mat &cameraMatrix) override;

    /**
     * Loads models based on list of names
     * @param modelNames The names of models to load
     * @return True on success
     */
    bool loadModels(const std::vector<std::string> &modelNames);


private:
    //QOpenGLContext *glContext; - Owned by the processing thread with one instance of this strategy to only have one context exist

    // OpenGL shader/fbo
    QOpenGLShaderProgram shaderProgram;
    QOpenGLFramebufferObject *fbo;

    // Model loading
    ModelLoader modelLoader;
    std::unordered_map<std::string, std::shared_ptr<ModelData>> models;

};

#endif //AR_SETTLERS_OPENGLRENDERSTRATEGY_H
