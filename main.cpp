#include "opencv2/opencv.hpp"
#include "stdio.h"
using namespace cv;

const int INCREASE_MAX_VALUE_KEY = 116; // t
const int DECREASE_MAX_VALUE_KEY = 103;// g
const int INCREASE_MIN_VALUE_KEY = 121; // y
const int DECREASE_MIN_VALUE_KEY = 104;// h
const int ESCAPE_KEY = 27;// ESC
int cameraVFOV = 90;
int cameraHHOV = 90;

double min_value = 30;
double max_value = 240;

bool isExiting = false;

int main(int argc, char** argv)
{
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if(!cap.open(0))
        return 0;
    while(!isExiting)
    {
        // === Input Managment ===
        int key_press = waitKey(5);
        switch(key_press) {
            case -1:
                break;
            case INCREASE_MAX_VALUE_KEY:
                max_value++;
                std::cout << "Max val: " << max_value << std::endl;
                break;
            case DECREASE_MAX_VALUE_KEY:
                max_value--;
                std::cout << "Max val: " << max_value << std::endl;
                break;
            case INCREASE_MIN_VALUE_KEY:
                min_value++;
                std::cout << "Min val: " << min_value << std::endl;
                break;
            case DECREASE_MIN_VALUE_KEY:
                max_value--;
                std::cout << "Min val: " << min_value << std::endl;
                break;
            case ESCAPE_KEY:
                std::cout << "Escape pressed!" << std::endl;
                isExiting = true;
                break;
            default:
                std::cout << "Unknown key pressed: " << key_press << std::endl;
        }

        
        Mat frame;
        cap >> frame;
        if( frame.empty() ) break; // end of video stream
        Mat3b hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        Mat1b mask;

        inRange(hsv, Scalar(0, 0, min_value), Scalar(255, 255, max_value), mask);

        SimpleBlobDetector detector;
 
        
        std::vector<KeyPoint> keypoints;
        detector.detect( mask, keypoints);
        
        
        Mat im_with_keypoints;
        drawKeypoints( mask, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        
        imshow("keypoints", im_with_keypoints );

        
    }
    // the camera will be c1losed automatically upon exit
    // cap.close();
    return 0;
}