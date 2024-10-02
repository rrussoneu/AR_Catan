//
// Created by Raphael Russo on 10/1/24.
//

#ifndef AR_SETTLERS_MESSAGEEMITTER_H
#define AR_SETTLERS_MESSAGEEMITTER_H
#pragma once
#include <QObject>

class MessageEmitter : public QObject {
    Q_OBJECT

public:

    explicit MessageEmitter(QObject* parent = nullptr)
            : QObject(parent) {}

    virtual ~MessageEmitter() {}

signals:
    // Signal for error
    void sendError(const QString &errorMessage);

    // Signal for generic message
    void sendMessage(const QString &message);
};

#endif //AR_SETTLERS_MESSAGEEMITTER_H
