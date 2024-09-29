//
// Created by Raphael Russo on 9/25/24.
//

// OpenGLRenderStrategy.cpp
#include "OpenGLRenderStrategy.h"
#include "../../Config.h"
#include <QDebug>


OpenGLRenderStrategy::OpenGLRenderStrategy()
        : fbo(nullptr) {
    /* Init called in process thread when OpenGLRenderStrategy is first needed.
     * Only one instance should exist to handle threading/ context.
     * All objects needing this strat get the pointer for it
     * */
}

OpenGLRenderStrategy::~OpenGLRenderStrategy() {
    if (fbo) {
        delete fbo;
    }
    for (auto& pair : models) {
        delete pair.second;
    }
}

void OpenGLRenderStrategy::prepareForRendering(const cv::Mat &frame) {


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

QMatrix4x4 OpenGLRenderStrategy::createProjectionMatrix(const cv::Mat &cameraMatrix, int width, int height, double nearPlane, double farPlane) {
    double fx = cameraMatrix.at<double>(0, 0);
    double fy = cameraMatrix.at<double>(1, 1);
    double cx = cameraMatrix.at<double>(0, 2);
    double cy = cameraMatrix.at<double>(1, 2);

    QMatrix4x4 projection; // Worth noting that the Qt matrix is column major like OpenGL
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

QMatrix4x4 OpenGLRenderStrategy::createModelViewMatrix(const cv::Vec3d &rvec, const cv::Vec3d &tvec) {
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

// Using these for now at least - can add the real files back in once finished

const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec3 aColor;
        // No textures for now
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

bool OpenGLRenderStrategy::initialize(int frameWidth, int frameHeight) {
    initializeOpenGLFunctions();

    if (!shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource) ||
        !shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource) ||
        !shaderProgram.link()) {
        qWarning() << "Shader program failed to compile or link:" << shaderProgram.log();
        return false;
    }

    // Load models
    // Maybe add longest road / largest army
    std::vector<std::string> modelNames = {
            "ore",
            "wool",
            "wood",
            "wheat",
            "brick"
    };
    if (!loadModels(modelNames)) {
        qWarning() << "Failed to load models.";
    }

    // Init fbo
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


void OpenGLRenderStrategy::render(ARObject *object, cv::Mat &frame,
                                  const std::vector<cv::Point2f> &markerCorners,
                                  const cv::Vec3d &rvec, const cv::Vec3d &tvec,
                                  const std::vector<double> &distCoeffs,
                                  const cv::Mat &cameraMatrix) {


    // Get the model name associated with the marker/ AR object
    std::string modelName = object->getName();

    // Get data for object model
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

void OpenGLRenderStrategy::finalizeRendering(cv::Mat &frame) {
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

    // Adjust frames if needed
    if (glFrameF.size() != frameBGRAF.size() || glFrameF.channels() != frameBGRAF.channels()) {
        cv::resize(glFrameF, glFrameF, frameBGRAF.size());
    }

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

bool OpenGLRenderStrategy::loadModels(const std::vector<std::string>& modelNames) {
    for (const auto &name : modelNames) {
        // Might refactor the directory - maybe come back to this
        std::string filePath = "AR_Objs/Rendering/Models/" + name + ".obj";
        if (!modelLoader.loadModel(name, filePath)) {
            qWarning() << "Failed to load model:" << QString::fromStdString(name);

        } else {
            models[name] = modelLoader.getModel(name);
        }

    }
    return true;
}
