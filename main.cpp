//
// Created by Raphael Russo on 9/22/24.
//

#include "Video/CameraInput.h"
#include <opencv2/highgui.hpp>

int main() {

    CameraInput cameraInput;
    if (!cameraInput.startStream()) {
        std::cerr << "Failed to start camera input" << std::endl;
        return -1;
    }
    cv::Mat frame;
    while (true) {
        if (cameraInput.getFrame(frame)) {
            cv::imshow("Camera Feed", frame);
        }

        if (cv::waitKey(30) == 27) {  // Exit on esc
            break;
        }
    }

    cameraInput.stopStream();
    return 0;
}
