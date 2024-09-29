//
// Created by Raphael Russo on 9/25/24.
// Generates a set of markers for the game of Catan - everything related to the markers can be fine tuned and adjusted as needed
// Right now the markers are quite large to make things easier
//

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <vector>
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

// Function to generate and save individual markers with category and ID in filename
void generateMarker(int markerID, aruco::Dictionary dictionary, int markerSize, const string &outputDir, const string &category) {
    Mat markerImage;
    // Generate the marker with the given ID and dictionary
    aruco::generateImageMarker(dictionary, markerID, markerSize, markerImage);

    // Create the directory if it doesn't exist
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    // Save the marker image to a file with the category name and marker ID
    string filename = outputDir + "/" + category + "_marker_" + to_string(markerID) + ".png";
    imwrite(filename, markerImage);
    cout << "Marker ID " << markerID << " (" << category << ") saved as " << filename << endl;
}

int main() {
    // Init the dictionary for the markers
    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_1000);

    // Marker size in pixels - can adjust
    // 3.8 cm is about 1.5 inches, assuming 40 px/cm (150 px total)
    int markerSize = 150;  // For 3.8 cm markers (150 pixels at 96ish DPI)

    // Directory to save marker images
    string outputDir = "markers";  //Create this directory if needed

    // Create markers for each game piece with category names
    vector<pair<string, vector<int>>> markers = {
            {"Roads_Blue", {1, 15}},
            {"Roads_Red", {16, 30}},
            {"Roads_Orange", {31, 45}},
            {"Roads_White", {46, 60}},
            {"Settlements_Blue", {61, 65}},
            {"Settlements_Red", {66, 70}},
            {"Settlements_Orange", {71, 75}},
            {"Settlements_White", {76, 80}},
            {"Cities_Blue", {81, 84}},
            {"Cities_Red", {85, 88}},
            {"Cities_Orange", {89, 92}},
            {"Cities_White", {93, 96}},
            {"Robber", {97}},
            {"Largest_Army", {98}},
            {"Longest_Road", {99}},
            {"Knight_Cards", {100, 113}},
            {"Monopoly_Cards", {114, 115}},
            {"Year_of_Plenty_Cards", {116, 117}},
            {"Road_Building_Cards", {118, 119}},
            {"Victory_Points", {120, 124}},
            {"Wood_Resource_Cards", {125}},
            {"Brick_Resource_Cards", {126}},
            {"Ore_Resource_Cards", {127}},
            {"Wheat_Resource_Cards", {128}},
            {"Wool_Resource_Cards", {129}},
            {"Wood_Hex", {130, 133}},
            {"Brick_Hex", {134, 136}},
            {"Ore_Hex", {137, 139}},
            {"Wheat_Hex", {140, 143}},
            {"Wool_Hex", {144, 147}}
    };

    // Iterate over the markers and generate them
    for (const auto &entry : markers) {
        string category = entry.first;
        vector<int> range = entry.second;

        // Generate each marker in the specified range
        for (int markerID = range[0]; markerID <= range.back(); markerID++) {
            generateMarker(markerID, dictionary, markerSize, outputDir, category);
        }
    }

    return 0;
}
