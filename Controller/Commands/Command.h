//
// Created by Raphael Russo on 9/24/24.
//

#ifndef AR_SETTLERS_COMMAND_H
#define AR_SETTLERS_COMMAND_H

#pragma once
#include <QString>

class Command {
public:
    virtual ~Command() = default;
    virtual QMap<QString, QVariant> run() = 0;
};


#endif //AR_SETTLERS_COMMAND_H
