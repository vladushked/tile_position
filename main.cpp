#include <iostream>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

float angle = 0;
int quartcircle = 0;

const float delta_theta = 0.35; // костанта для тетта
const float delta_rho = 20; // костанта для ро

void imgProc(Mat img);
float findVertical(Mat img);
void angleVar(float cur, float* prev, int* semi);

int main()
{
    //вывод видео
    Mat frame;
    float angle;
    string angle_text;

    VideoCapture capture("/home/vladushked/Documents/code/hydronautics/videos/bottom__10.avi");
    namedWindow("video", WINDOW_AUTOSIZE);

    for (;;) {

        capture.read(frame); //read frame
        cvtColor(frame,frame,COLOR_BGR2GRAY); // frame color to grayscale
        if (frame.empty()){ //check if frame empty
            cerr << "Empty frame!";
        }
        imgProc(frame); //image processing
        //vertical
        //angle_text = std::to_string(findVertical(frame) * 180 / CV_PI);
        //putText(frame,angle_text,Point(10,110),CV_FONT_NORMAL,1,255);
        //global angle
        angleVar(findVertical(frame), &angle, &quartcircle);
        angle_text = std::to_string(angle * 180 / CV_PI + quartcircle * 90);
        putText(frame,std::to_string(angle * 180 / CV_PI),Point(10,30),CV_FONT_NORMAL,1,255);
        putText(frame,std::to_string(quartcircle*45),Point(10,70),CV_FONT_NORMAL,1,255);
        putText(frame,angle_text,Point(10,110),CV_FONT_NORMAL,1,255);

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

float findVertical(Mat img)
{
    vector<float> temp_theta;
    vector<Vec2f> lines;
    float average_sum = 0, average = 0, vertical = 0, l_corn_theta, r_corn_theta;
    int average_counter = 0;

    HoughLines(img, lines, 1, CV_PI/180, 100);

    for (size_t i = 0; i < lines.size(); i++){ // находим линию не близкую к границам
        l_corn_theta = lines[i][1] - delta_theta; //дельты ро и тета
        r_corn_theta = lines[i][1] + delta_theta;
        //проверка на выход за границы
        if ((l_corn_theta < 0) | (r_corn_theta > CV_PI)) continue;
        else break;
    }

    for (size_t i = 0; i < lines.size(); i++){
        float theta = lines[i][1]; // текущие значения
        if ((theta > l_corn_theta) and (theta < r_corn_theta)){
            temp_theta.push_back(theta);
            average_sum += theta;
            average_counter++;
        }
    }
    average = average_sum/average_counter; //average
    if ((average > 0.785) and (average < 2.356)) vertical = average - CV_PI/2; // проверка на вертикальность
    else if (average > 2.356) vertical = average - CV_PI;
    else vertical = average;

    return vertical;
}

void angleVar(float cur, float* prev, int* quart)
{
    if (((cur > 0) & (*prev > 0)) | ((cur < 0) & (*prev < 0))){
        *prev = cur;
    }
    else{
        float delta = cur - *prev;
        if (fabs(delta) < 0.35) *prev = cur;
        else if (delta > 0){
            *prev = cur;
            *quart -= 1;
        }
        else{
            *prev = cur;
            *quart += 1;
        }
    }
}
