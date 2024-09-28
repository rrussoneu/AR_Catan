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
    OpenGLRenderStrategy(QOpenGLContext* context);
    ~OpenGLRenderStrategy();

    void prepareForRendering(const cv::Mat &frame) {
        if (!fbo || fbo->size() != QSize(frame.cols, frame.rows)) {
            if (fbo) delete fbo;
            QOpenGLFramebufferObjectFormat format;
            format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
            fbo = new QOpenGLFramebufferObject(frame.cols, frame.rows, format);
            //qDebug() << "Created new fbo size:" << fbo->size();
        }

        if (!fbo->bind()) {
            qWarning() << "Failed to bind FBO in prepareForRendering";
            return;
        }



        glViewport(0, 0, frame.cols, frame.rows);
        glEnable(GL_DEPTH_TEST);
        //glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Blue bg
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

    void renderMarker(const cv::Vec3d &rvec, const cv::Vec3d &tvec,
                      const std::vector<double> &distCoeffs, const cv::Mat &cameraMatrix) {
        glContext->makeCurrent(glContext->surface());

        shaderProgram.bind();
        vao.bind();
        vbo.bind();

        QMatrix4x4 projectionMatrix = createProjectionMatrix(cameraMatrix, fbo->width(), fbo->height(), 0.1, 100.0);
        QMatrix4x4 modelViewMatrix = createModelViewMatrix(rvec, tvec);

        // Apply scaling after setting up model view matrix
        float scale = Config::getInstance().getMarkerLength();
        modelViewMatrix.scale(scale, scale, scale);

        shaderProgram.setUniformValue("uProjectionMatrix", projectionMatrix);
        shaderProgram.setUniformValue("uModelViewMatrix", modelViewMatrix);

        // Define triangle centered at origin
        float vertices[] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };
        vbo.allocate(vertices, sizeof(vertices));

        shaderProgram.enableAttributeArray("aPosition");
        shaderProgram.setAttributeBuffer("aPosition", GL_FLOAT, 0, 3);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        vbo.release();
        vao.release();
        shaderProgram.release();

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            qWarning() << "OpenGL error after rendering:" << error;
        }

    }

    bool initialize() {
        initializeOpenGLFunctions();

        if (!shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource) ||
            !shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource) ||
            !shaderProgram.link()) {
            qWarning() << "Shader program failed to compile or link:" << shaderProgram.log();
            return false;
        }

        vao.create();
        vbo.create();

        return true;
    }

    // Testing shaders instead of the files
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPosition;
        uniform mat4 uProjectionMatrix;
        uniform mat4 uModelViewMatrix;
        void main() {
            gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 fragColor;
        void main() {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0); // Solid red
        }
    )";

    void finalizeRendering(cv::Mat &frame) {
        fbo->release();
        QImage glImage = fbo->toImage();

        cv::Mat glFrame(glImage.height(), glImage.width(), CV_8UC4, (void*)glImage.bits(), glImage.bytesPerLine());
        cv::Mat frameBGRA;
        cv::cvtColor(frame, frameBGRA, cv::COLOR_BGR2BGRA);

        cv::addWeighted(frameBGRA, 1.0, glFrame, 0.5, 0.0, frameBGRA);

        cv::cvtColor(frameBGRA, frame, cv::COLOR_BGRA2BGR);

    }

    void render(ARObject *object, cv::Mat &frame,
                const std::vector<cv::Point2f> &markerCorners,
                const cv::Vec3d &rvec, const cv::Vec3d &tvec,
                const std::vector<double> &distCoeffs,
                const cv::Mat &cameraMatrix) override;

    //bool initialize();
    bool loadModels(const std::vector<std::string> &modelNames);


private:
    QOpenGLContext *glContext;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLFramebufferObject *fbo;
    ModelLoader modelLoader;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;

    //std::unordered_map<std::string, ModelData*> models;

    void setupShaders();
    void setupMatrices(const cv::Mat& cameraMatrix, const std::vector<double>& distCoeffs,
                       const cv::Vec3d& rvec, const cv::Vec3d& tvec,
                       int frameWidth, int frameHeight);
};

#endif //AR_SETTLERS_OPENGLRENDERSTRATEGY_H
