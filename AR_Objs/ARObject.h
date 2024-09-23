//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_AROBJECT_H
#define AR_SETTLERS_AROBJECT_H

#pragma once
#include <opencv2/opencv.hpp>

class ARObject {
public:
    ARObject() {}
    virtual ~ARObject() = default;
    virtual void render(cv::Mat &frame) =0;
};
#endif //AR_SETTLERS_AROBJECT_H
