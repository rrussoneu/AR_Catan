//
// Created by Raphael Russo on 9/22/24.
//

#ifndef AR_SETTLERS_HOMOGRAPHYOBJ_H
#define AR_SETTLERS_HOMOGRAPHYOBJ_H
#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class HomographyObj {

protected:
    cv::Mat &homographyMat;
    std::vector<int> homographyMarkerIDs;
};

#endif //AR_SETTLERS_HOMOGRAPHYOBJ_H
