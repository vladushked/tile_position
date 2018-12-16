#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

void imgProc(Mat img);
void findLines(Mat img);

int main()
{
    //вывод видео
    Mat frame;
    VideoCapture capture("/home/vladushked/Documents/code/hydronautics/videos/bottom__10.avi");
    namedWindow("video", WINDOW_AUTOSIZE);

    for (;;) {
        capture.read(frame); //read frame

        cvtColor(frame,frame,COLOR_BGR2GRAY); // frame color to grayscale

        if (frame.empty()){ //check if frame empty
            cerr << "Empty frame!";

        }

        imgProc(frame); //image processing
        findLines(frame);

        imshow("video",frame);

        waitKey(0);

    }
    return 0;
}

void imgProc(Mat img) //обработка изображения
{
    blur(img, img, Size(9,9));
    adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, 2);
    Canny( img, img, 200, 600, 3);
}

void findLines(Mat img)
{
    vector<Vec2f> lines;
    HoughLines(img, lines, 1, CV_PI/180, 100);

    cout << "Before sort:" << endl;

    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];

        cout << lines[i][0] << " " << lines[i][1] << endl;
        if (theta < lines[0][1]);

        /*
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 2000*(-b));
        pt1.y = cvRound(y0 + 2000*(a));
        pt2.x = cvRound(x0 - 2000*(-b));
        pt2.y = cvRound(y0 - 2000*(a));
        line( img, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
        */
    }
    cout << endl;


}
