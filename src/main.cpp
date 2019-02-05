#include "../include/TileAngle.h"

static const std::string OPENCV_WINDOW = "tile_window";

int main()
{
    TileAngle tracker;
    //cv::VideoCapture capture(1);
    cv::VideoCapture capture("/home/vladushked/Documents/code/hydronautics/videos/bottom__10.avi");
    cv::Mat frame;
    cv::namedWindow(OPENCV_WINDOW, CV_WINDOW_AUTOSIZE);

    for (;capture.read(frame);){

        tracker.processing(frame);
        float globalAngle, verticalAngle;
        globalAngle = tracker.globalAngle();
        verticalAngle = tracker.verticalAngle();

        cv::imshow(OPENCV_WINDOW, frame);
        cv::waitKey(3);
    }
    cv::destroyWindow(OPENCV_WINDOW);
    return 0;
}

