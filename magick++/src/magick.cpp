#include <Magick++.h>
#include <iostream>
#include <vector>
#include <cstdio> // for remove
#include <filesystem> // C++17 for handling files
#include "win_dir.h"
using namespace std;
using namespace Magick;
namespace fs = std::filesystem;
int getFileNumInData(std::string floder_path);

int main() {
    InitializeMagick(nullptr);
    std::string gif_folder = ".\\gif\\";
    std::string gif_name = "dijistra.gif";
    std::string outputFile = gif_folder + gif_name;
    if (!CreateDirectoryIfNotExists(gif_folder)) {
        std::cout << "creat gif folder faided!" << std::endl;
            return 0;
    }
    int delay = 1;
    std::string images_path = "..\\dijistra\\numbered_images\\";
    vector<Image> imageList;
    int frameCount = 0;
    int image_num = getFileNumInData(images_path);
    while (true) {
        string file_path = images_path + to_string(frameCount) + ".png";
        if (!fs::exists(file_path)) {
            break;
        }
        Image img;
        img.read(file_path);
        img.animationDelay(delay);//
        imageList.push_back(img);
        std::cout << "Picture #" << frameCount << " has been read by magick++\n";
        frameCount = frameCount + 500;
        //add last picture to imageList
        if (frameCount > image_num - 1) {
            img.read(images_path + to_string(image_num - 1) + ".png");
            img.animationDelay(delay);
            imageList.push_back(img);
            std::cout << "Picture #last" << " has been read by magick++\n";
        }
    }
    //
    try {
        std::cout << " Writting to a gif file, please wait...\n";
        writeImages(imageList.begin(), imageList.end(), outputFile);
        cout << "GIF created successfully: " << outputFile << endl;
    }
    catch (Exception& error_) {
        cerr << "Error creating GIF: " << error_.what() << endl;
        return 1;
    }

    return 0;
}

int getFileNumInData(std::string floder_path)
{
    fs::directory_iterator list_(floder_path);
    int i = 0;
    for (auto& it : list_)
    {
        i++;
        cout << it.path().filename() << endl;
    }
    return i;
}
