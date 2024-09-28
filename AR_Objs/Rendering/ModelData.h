//
// Created by Raphael Russo on 9/27/24.
//

#ifndef AR_SETTLERS_MODELDATA_H
#define AR_SETTLERS_MODELDATA_H

#pragma once

#include <vector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

struct ModelData {
    int vertexCount;

    // OpenGL buffers
    QOpenGLBuffer vertexBuffer;     // Vertex positions
    QOpenGLBuffer colorBuffer; // Colors
    QOpenGLBuffer normalBuffer;     // Normals
    QOpenGLBuffer texCoordBuffer;   // Texture coords
    QOpenGLVertexArrayObject vao;   // Vertex Array Object

    ModelData()
            : vertexCount(0),
              vertexBuffer(QOpenGLBuffer::VertexBuffer),
              normalBuffer(QOpenGLBuffer::VertexBuffer),
              texCoordBuffer(QOpenGLBuffer::VertexBuffer) {}
};

#endif //AR_SETTLERS_MODELDATA_H
