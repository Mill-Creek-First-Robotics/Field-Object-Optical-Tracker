#include "opencv2/opencv.hpp"
#include "stdio.h"
using namespace cv;

const int INCREASE_MAX_VALUE_KEY = 116; // t
const int DECREASE_MAX_VALUE_KEY = 103; // g
const int INCREASE_MIN_VALUE_KEY = 121; // y
const int DECREASE_MIN_VALUE_KEY = 104; // h
const int SWITCH_CAMERA_KEY = 99; // c
const int ESCAPE_KEY = 27;// ESC

int cameraIndex = 0;

int cameraVFOV = 90;
int cameraHHOV = 90;

double min_value = 200;
double max_value = 255;

bool isExiting = false;

int main(int argc, char** argv)
{
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if(!cap.open(cameraIndex))
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
                min_value--;
                std::cout << "Min val: " << min_value << std::endl;
                break;
            case ESCAPE_KEY:
                std::cout << "Escape pressed!" << std::endl;
                isExiting = true;
                break;
            case SWITCH_CAMERA_KEY:
                cap.release();
                cameraIndex++;
                if(cameraIndex != 0 && !cap.open(cameraIndex)){
                    cameraIndex = 0;
                    cap.open(cameraIndex);
                }
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
        Mat1b inv_mask;
        bitwise_not(mask, inv_mask);  // Invert mask

        Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(); 
        
        std::vector<KeyPoint> keypoints;
        detector->detect( inv_mask, keypoints);
        
        Mat im_with_keypoints;
        drawKeypoints( mask, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        
        imshow("keypoints", im_with_keypoints );

        
    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}