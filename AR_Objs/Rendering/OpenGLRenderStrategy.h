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
    OpenGLRenderStrategy(QOpenGLContext *context);
    ~OpenGLRenderStrategy();

    int getType() override {
        return 2;
    }

    void prepareForRendering(const cv::Mat &frame) {


        if (!fbo->bind()) {
            qWarning() << "Failed to bind FBO in prepareForRendering";
            return;
        }

        // OpenGL prep
        // Set viewport to match FBO size
        glViewport(0, 0, fbo->width(), fbo->height());

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }


    QMatrix4x4 createProjectionMatrix(const cv::Mat &cameraMatrix, int width, int height, double nearPlane, double farPlane) {
        double fx = cameraMatrix.at<double>(0, 0);
        double fy = cameraMatrix.at<double>(1, 1);
        double cx = cameraMatrix.at<double>(0, 2);
        double cy = cameraMatrix.at<double>(1, 2);

        QMatrix4x4 projection;
        projection.setToIdentity();

        // Adjust for coordinate system differences with OpenCV and OpenGL
        projection(0, 0) = 2 * fx / width;
        projection(1, 1) = 2 * fy / height;
        projection(0, 2) = 2 * (0.5 * width - cx) / width;
        projection(1, 2) = 2 * (cy - 0.5 * height) / height;
        projection(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
        projection(2, 3) = -2 * farPlane * nearPlane / (farPlane - nearPlane);
        projection(3, 2) = -1;
        projection(3, 3) = 0;

        return projection;
    }





    QMatrix4x4 createModelViewMatrix(const cv::Vec3d &rvec, const cv::Vec3d &tvec) {
        cv::Mat rotMat;
        cv::Rodrigues(rvec, rotMat);

        QMatrix4x4 modelView;
        modelView.setToIdentity();

        // Convert rotation matrix from OpenCV to Qt
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                modelView(i, j) = rotMat.at<double>(i, j);
            }
        }

        // Set translation
        modelView(0, 3) = tvec[0];
        modelView(1, 3) = tvec[1];
        modelView(2, 3) = tvec[2];

        // Adjust for coordinate system differences
        QMatrix4x4 coordinateTransform;
        coordinateTransform.setToIdentity();
        coordinateTransform.scale(1.0f, -1.0f, -1.0f); // Flip Y and Z axes

        return coordinateTransform * modelView;
    }



    bool initialize(int frameWidth, int frameHeight) {
        initializeOpenGLFunctions();

        if (!shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource) ||
            !shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource) ||
            !shaderProgram.link()) {
            qWarning() << "Shader program failed to compile or link:" << shaderProgram.log();
            return false;
        }

        // Load models
        // To Do:
        // Wood, brick, wheat, maybe longest road / largest army
        std::vector<std::string> modelNames = {
                "ore",
                "wool",
                "wood",
                "wheat",
                "brick"
        };
        if (!loadModels(modelNames)) {
            qWarning() << "Failed to load models.";
            //return false;
        }

        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        fbo = new QOpenGLFramebufferObject(frameWidth, frameHeight, format);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);



        return true;
    }


    // Ignore the shader files - just use these for now at least

    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec3 aColor;
        // No textures right now
        // layout(location = 3) in vec2 aTexCoord;

        uniform mat4 uProjectionMatrix;
        uniform mat4 uModelViewMatrix;

        out vec3 fragNormal;
        out vec3 fragColor;
        // out vec2 fragTexCoord;

        void main() {
            gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
            fragNormal = mat3(uModelViewMatrix) * aNormal;
            fragColor = aColor;
            // fragTexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 fragNormal;
        in vec3 fragColor;
        // Maybe add in textures depending on if new models are made
        // in vec2 fragTexCoord;


        out vec4 fragColorOut;

        void main() {
            // Normalize the normal vector
            vec3 normal = normalize(fragNormal);

            // Light properties
            vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0)); // Light coming from camera direction
            vec3 ambientLight = vec3(0.3); // Ambient light

            // Diffuse lighting
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * fragColor;

            // Combine ambient and diffuse lighting
            vec3 finalColor = ambientLight * fragColor + diffuse;

            fragColorOut = vec4(finalColor, 1.0);
            //fragColorOut = vec4(1.0, 0.0, 0.0, 1.0); // Solid red, fully opaque - for testing purposes
        }

    )";

    void finalizeRendering(cv::Mat &frame) {
        fbo->release();

        // Fbo to qImage
        QImage glImage = fbo->toImage();

        // Convert QImage to mat
        cv::Mat glFrame(glImage.height(), glImage.width(), CV_8UC4, (void*)glImage.bits(), glImage.bytesPerLine());

        // Save the OpenGL img for debug if needed
        //cv::imwrite("testAlpha.png", glFrame);

        // RGBA frame
        cv::Mat frameBGRA;
        cv::cvtColor(frame, frameBGRA, cv::COLOR_BGR2BGRA);

        // Convert images to floats
        cv::Mat glFrameF, frameBGRAF;
        glFrame.convertTo(glFrameF, CV_32FC4, 1.0 / 255.0);
        frameBGRA.convertTo(frameBGRAF, CV_32FC4, 1.0 / 255.0);

        // Split glFrameF into individual channels
        std::vector<cv::Mat> glChannels(4);
        cv::split(glFrameF, glChannels);

        // Get alpha channel
        cv::Mat alphaChannel = glChannels[3];

        // 4 channel alpha mtx
        std::vector<cv::Mat> alphaChannels(4, alphaChannel);
        cv::Mat alphaMat;
        cv::merge(alphaChannels, alphaMat);

        // Alpha blend
        cv::Mat blendedF = glFrameF.mul(alphaMat) + frameBGRAF.mul(cv::Scalar(1.0, 1.0, 1.0, 1.0) - alphaMat);

        // Convert back BGRA/8 bit
        blendedF.convertTo(frameBGRA, CV_8UC4, 255.0);

        // Convert back to BGR
        cv::cvtColor(frameBGRA, frame, cv::COLOR_BGRA2BGR);

    }

    void render(ARObject *object, cv::Mat &frame,
                const std::vector<cv::Point2f> &markerCorners,
                const cv::Vec3d &rvec, const cv::Vec3d &tvec,
                const std::vector<double> &distCoeffs,
                const cv::Mat &cameraMatrix) override;

    bool loadModels(const std::vector<std::string> &modelNames);


private:
    QOpenGLContext *glContext;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLFramebufferObject *fbo;
    ModelLoader modelLoader;
    std::unordered_map<std::string, ModelData*> models;

};

#endif //AR_SETTLERS_OPENGLRENDERSTRATEGY_H
