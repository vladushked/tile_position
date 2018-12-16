#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

void imgProc(Mat img);
void findLines(Mat img);

const float delta_theta = 0.35; // костанта для тетта
const float delta_rho = 20; // костанта для ро

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
    bool fl_th_out = 0;

    vector<float> temp_lines_rho, temp_lines_theta;
    vector<Vec2f> lines, horizontal_lines, vertical_lines;

    HoughLines(img, lines, 1, CV_PI/180, 100); // преобразование Хафа. нахождение линий

    cout << "Before sort:" << endl;

    float l_corn_theta = lines[0][1] - delta_theta; //дельты ро и тета
    float r_corn_theta = lines[0][1] + delta_theta;
    float l_corn_rho = lines[0][0] - delta_rho;
    float r_corn_rho = lines[0][0] + delta_rho;



    if (l_corn_theta < 0) l_corn_theta += 180; // проверка на выход дельты тетта из границ интервала тетта(0..180 град)
    else if (r_corn_theta > 180){
        fl_th_out = 1; // true если вышло
        r_corn_theta -= 180;
    }

    for (size_t i = 0; i < lines.size(); i++)
    {
        cout << lines[i][0] << " " << lines[i][1] << endl; // вывод исходного массива
        float rho = lines[i][0], theta = lines[i][1]; // текущие значения

        if (!fl_th_out){ // если не вышло за границы
            if ((theta > l_corn_theta) and (theta < r_corn_theta)){
                temp_lines_rho.push_back(rho);
                temp_lines_theta.push_back(theta);
            }
            else {

            }
        }
        else if (((theta > l_corn_theta) and (theta < CV_PI)) or ((theta > 0) and (theta < r_corn_theta))){ // если дельта вышла за границы 0..180 град

        }

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

    for (size_t i = 0; i < temp_lines_rho.size(); i++)
        cout << temp_lines_rho[i] << " " << temp_lines_theta[i] << endl; // вывод temp массива

}
