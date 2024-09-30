//
// Created by Raphael Russo on 9/22/24.
// An abstract class for video input types
//

#ifndef AR_SETTLERS_VIDEOINPUT_H
#define AR_SETTLERS_VIDEOINPUT_H

#pragma once
#include <opencv2/opencv.hpp>

class VideoInput {
public:
    virtual ~VideoInput() {}

    // Starts video capture, returns true if successful
    virtual bool startStream() = 0;

    // Stops video capture
    virtual void stopStream() = 0;

    /**
     * Gets data from video source and stores in frame
     * @param frame CV mat for frame data
     * @return True on success
     */
    virtual bool getFrame(cv::Mat &frame) = 0;

protected:
    // Is currently capturing video stream flag
    bool isStreaming = false;
};


#endif //AR_SETTLERS_VIDEOINPUT_H
