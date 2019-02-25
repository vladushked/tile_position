#include <iostream>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

const std::string OPENCV_WINDOW = "tile_window";
const float delta_theta = 0.35; // костанта для тетта
const float delta_rho = 20; // костанта для ро

std::vector<float> lines_vertical, lines_horizontal;
int vertical_counter, horizontal_counter; //счетчики линий
float vertical = 0;
float angle = 0;
int quartcircle = 0;

void imgPrep(cv::Mat &img);
void finder(cv::Mat &img);
void linesTracker();
void angleVar(float cur, float* prev, int* quart);
void processing(cv::Mat &img);

int main()
{

    //cv::VideoCapture capture(1);
    cv::VideoCapture capture("/home/vladushked/Documents/code/hydronautics/videos/bottom__10.avi");
    cv::Mat frame;
    cv::namedWindow(OPENCV_WINDOW, CV_WINDOW_AUTOSIZE);

    for (size_t i=1 ;i>0;i++){
        std::cout << std::endl << "iteration " << i << std::endl <<"-------------"<< std::endl;
        capture.read(frame);
        std::cout << "frame readed, ";
         processing(frame);
        cv::imshow(OPENCV_WINDOW, frame);
        cv::waitKey(0);
    }
    cv::waitKey(0);
    cv::destroyWindow(OPENCV_WINDOW);
    return 0;
}

void imgPrep(cv::Mat &img) //обработка изображения
{
    cvtColor(img,img,cv::COLOR_BGR2GRAY); // frame color to grayscale
    blur(img, img, cv::Size(9,9));
    adaptiveThreshold(img, img, 255,cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, 2);
    Canny(img, img, 200, 600, 3);
    std::cout << "img prepared, ";
}

void finder(cv::Mat &img)
{
    std::vector<float> temp_theta, temp_vertical, temp_horizontal;
    std::vector<cv::Vec2f> lines;
    float average_sum = 0, average, l_corn_theta = 0, r_corn_theta = 0;
    int average_counter = 0;

    HoughLines(img, lines, 1, CV_PI/180, 80);
    std::cout << "hough, ";

    //vertical angle
    for (size_t i = 0; i < lines.size(); i++){ // находим линию не близкую к границам
        l_corn_theta = lines[i][1] - delta_theta; //дельта тета
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

    l_corn_theta = vertical - delta_theta; //дельта тета
    r_corn_theta = vertical + delta_theta;

    //отбор линий по углу
    for (size_t i = 0; i < lines.size(); i++){
        float theta = lines[i][1], rho = lines[i][0]; // текущие значения
        if (rho >= 0){
            if ((theta > l_corn_theta) and (theta < r_corn_theta)){
                temp_vertical.push_back(rho);
                //std::cout << rho << std::endl;
            }
            else {
                temp_horizontal.push_back(rho);
                //std::cout << "\t" << rho << std::endl;
            }
        }
    }
    //sorting lines
    std::sort(temp_vertical.begin(), temp_vertical.end());
    std::sort(temp_horizontal.begin(), temp_horizontal.end());
    std::cout << "sorted, ";

    //filtration

    std::cout << "lines_vertical:"<< std::endl;

    for (size_t i = 0; i < temp_vertical.size(); i++){
        if (temp_vertical.size() == 0) break;
        else if (i == 0){
            lines_vertical.push_back(temp_vertical[i]);
            std::cout << temp_vertical[i] << std::endl;
            continue;
        }

        if ((temp_vertical[i] - temp_vertical[i-1]) < delta_rho) continue;
        else{
            lines_vertical.push_back(temp_vertical[i]);
            std::cout << temp_vertical[i] << std::endl;
        }
    }

    std::cout << "lines_horizontal:"<< std::endl;

    for (size_t i = 0; i < temp_horizontal.size(); i++){
        if (temp_horizontal.size() == 0) break;
        else if (i == 0){
            lines_horizontal.push_back(temp_horizontal[i]);
            std::cout << temp_horizontal[i] << std::endl;
            continue;
        }

        if ((temp_horizontal[i] - temp_horizontal[i-1]) < delta_rho) continue;
        else{
            lines_horizontal.push_back(temp_horizontal[i]);
            std::cout << temp_horizontal[i] << std::endl;
        }
    }

    std::cout << "filtered, ";
}

void angleVar(float cur, float* prev, int* quart)
{
    if (((cur > 0) & (*prev > 0)) | ((cur < 0) & (*prev < 0))){
        *prev = cur;
    }
    else{
        float delta = cur - *prev;
        if (std::fabs(delta) < 0.35) *prev = cur;
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

void processing(cv::Mat &img)
{
    imgPrep(img); //image processing
    finder(img);
    angleVar(vertical, &angle, &quartcircle);


    std::string angle_text;
    //vertical
    angle_text = std::to_string(angle);
    cv::putText(img,angle_text + " rad",cv::Point(10,30),CV_FONT_NORMAL,1,255);
    //global angle
    angle_text = std::to_string(angle * 180 / CV_PI + quartcircle * 90);
    cv::putText(img,angle_text + " grad",cv::Point(10,70),CV_FONT_NORMAL,1,255);
}
