//
// Created by Raphael Russo on 9/29/24.
// Generates a charuco board and saves as an image to be used for camera calibration
//


#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/imgcodecs.hpp>

/**
 * Generates and saves the board
 */
int main() {

    // CV bord init
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_1000);
    cv::aruco::CharucoBoard board = cv::aruco::CharucoBoard(cv::Size(10, 14), 0.05, 0.03, dictionary); // 10x14 grid, 5cm square size, 3cm marker size

    // Create image of board
    cv::Mat boardImage;
    board.generateImage(cv::Size(1000, 800), boardImage, 10, 1);

    // Save board
    std::string bs;
    bs = "BoardImage" + std::to_string(000) + ".jpg";
    cv::imwrite(bs, boardImage);

    return 0;

}