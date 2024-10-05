//
// Created by Raphael Russo on 9/25/24.
//

#include "ModelLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <QOpenGLFunctions>
#include <QDebug>

ModelLoader::ModelLoader(QObject *parent) {
}

ModelLoader::~ModelLoader() {
}

bool ModelLoader::loadModel(const std::string &name, const std::string &filePath) {
    Assimp::Importer importer;

    // Import the model file with Assimp
    const aiScene *scene = importer.ReadFile(filePath,
                                             aiProcess_Triangulate |
                                             aiProcess_GenSmoothNormals |
                                             aiProcess_FlipUVs |
                                             aiProcess_JoinIdenticalVertices);

    if (!scene || !scene->HasMeshes()) {
        qWarning() << "Assimp failed to load model:" << importer.GetErrorString();
        return false;
    }
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qWarning() << "Assimp error:" << importer.GetErrorString();
        return false;
    }

    // Load first mesh - models are simple and should in theory only have one
    aiMesh *mesh = scene->mMeshes[0];

    std::shared_ptr<ModelData> modelData =std::make_shared<ModelData>();
    modelData->vertexCount = mesh->mNumFaces * 3; // Triangulated

    // Extract vertex data
    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<float> normals;


    // Get the material associated with the mesh
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // Get the diffuse color from the material
    aiColor4D diffuseColor(1.0f, 1.0f, 1.0f, 1.0); // Default to white
    if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor)) {
        qWarning() << "Failed to get diffuse color from material. Using default color.";
    }
    diffuseColor.a = 1.0f;

    // For each triangle
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        // Each face should have exactly 3 indices given triangulation
        for (unsigned int j = 0; j < 3; ++j) {
            unsigned int index = face.mIndices[j];

            // Vertex positions
            aiVector3D aiVertex = mesh->mVertices[index];
            vertices.push_back(aiVertex.x);
            vertices.push_back(aiVertex.y);
            vertices.push_back(aiVertex.z);

            // Normals
            if (mesh->HasNormals()) {
                aiVector3D aiNormal = mesh->mNormals[index];
                normals.push_back(aiNormal.x);
                normals.push_back(aiNormal.y);
                normals.push_back(aiNormal.z);
            } else {
                // If no normals use a default normal
                normals.push_back(0.0f);
                normals.push_back(0.0f);
                normals.push_back(1.0f);
            }


            // Vertex colors from mat diffuse color
            colors.push_back(diffuseColor.r);
            colors.push_back(diffuseColor.g);
            colors.push_back(diffuseColor.b);
        }
    }

    // Init OpenGL functions
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();

    // Create and bind VAO
    modelData->vao.create();
    modelData->vao.bind();



    // Vertices
    modelData->vertexBuffer.create();
    modelData->vertexBuffer.bind();
    modelData->vertexBuffer.allocate(vertices.data(), vertices.size() * sizeof(float));
    glFuncs->glEnableVertexAttribArray(0);
    glFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    modelData->vertexBuffer.release();

    // Normals
    if (!normals.empty()) {
        modelData->normalBuffer.create();
        modelData->normalBuffer.bind();
        modelData->normalBuffer.allocate(normals.data(), normals.size() * sizeof(float));
        glFuncs->glEnableVertexAttribArray(1);
        glFuncs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        modelData->normalBuffer.release();
    }

    // Colors
    modelData->colorBuffer.create();
    modelData->colorBuffer.bind();
    modelData->colorBuffer.allocate(colors.data(), colors.size() * sizeof(float));
    glFuncs->glEnableVertexAttribArray(2);
    glFuncs->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    modelData->colorBuffer.release();


    // Unbind VAO
    modelData->vao.release();

    // Store the model in the map
    models[name] = modelData;

    qDebug() << "Model loaded:" << QString::fromStdString(name) << "Vertex Count:" << modelData->vertexCount;

    return true;
}

std::shared_ptr<ModelData> ModelLoader::getModel(const std::string &name) {
    auto it = models.find(name);
    if (it != models.end()) {
        return it->second;
    }
    return nullptr;
}