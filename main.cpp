#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

void imgProc(Mat img);


int main()
{
    //вывод видео
    Mat frame;
    VideoCapture capture("../bottom__10.avi");
    namedWindow("video", WINDOW_AUTOSIZE);

    for (;;) {
        capture.read(frame);
        cvtColor(frame,frame,COLOR_BGR2GRAY); // color to gray

        if (frame.empty()){
            cout << "Empty frame!";
            break;
        }
        //image processing
        imgProc(frame);

        imshow("video",frame);
        waitKey(33);
        if (waitKey(5) >= 0)
            break;
    }
    return 0;
}

void imgProc(Mat img)
{
    blur(img, img, cv::Size(9,9));
    adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, 2);
    Canny( img, img, 200, 600, 3);
}
