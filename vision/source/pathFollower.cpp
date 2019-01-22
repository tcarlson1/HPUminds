#include <iostream>
#include <stdlib.h>
#include <string>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv/cv.hpp"
#include "../Dependencies/jsoncpp-master/dist/json/json.h"
#include "../Dependencies/jsoncpp-master/dist/json/json-forwards.h"
#include "../Dependencies/jsoncpp-master/dist/jsoncpp.cpp"

using namespace std;
using namespace cv;

//prototypes
void on_low_hue(int, void *);
void on_high_hue(int, void *);
void on_max_value(int, void *);
void on_blur_size(int, void *);
void on_canny_thresh(int, void *);
void on_canny_ratio(int, void *);

//globals
int low_Hue = 0;
int high_Hue = 100;
int max_value = 230;
int blur_size = 3;
int canny_thresh = 80;
int canny_ratio = 2;

RNG rng(12345);
const int center = 80;
const int padding = 12;

int main(int argc, char const *argv[])
{
    cout << CV_MAJOR_VERSION << "." <<CV_MINOR_VERSION;

    VideoCapture cap;
    Mat frame, maskedFrame, tempProcessingImg, tempMask1, tempMask2;
    if (argc < 2) {
        cerr << "please specify file to process\n";
    }

    const string source = argv[1];


    if (!cap.open(source)) {
        cout << "Not Opened" << endl;
        return -1;
    }

    namedWindow("Video Capture", WINDOW_NORMAL);
    namedWindow("maskedFrame", WINDOW_NORMAL);
    createTrackbar("Low Hue","maskedFrame", &low_Hue, 255, on_low_hue);
    createTrackbar("High Hue","maskedFrame", &high_Hue, 255, on_low_hue);
    createTrackbar("Max Value","maskedFrame", &max_value, 255, on_max_value);
    createTrackbar("Blur","maskedFrame", &blur_size, 10, on_blur_size);
    //createTrackbar("canny thresh","maskedFrame", &canny_thresh, 255, on_canny_thresh);
    //createTrackbar("canny ratio","maskedFrame", &canny_ratio, 5, on_canny_ratio);

    //loop through video file
    int frame_counter = 0;
    while((char)waitKey(1)!='q'){
        //string to send to brain
        string Movement;
        cap >> frame;

        frame_counter++;
        //break if at the end of the video file
        if(frame_counter == cap.get(CV_CAP_PROP_FRAME_COUNT)) {
            cout << "restarting video\n";
            frame_counter = 0;
            cap.set(CV_CAP_PROP_POS_FRAMES, 0);
        }

        //rotate the image
        //transpose(frame, frame);
        //flip(frame, frame, 1);
        resize(frame, frame, Size(160,80));
        medianBlur(frame, frame, (blur_size * 2) + 1);


        // ** mask out the path **
        //convert to hsv
        cvtColor(frame, tempProcessingImg, COLOR_RGB2HSV);
        //medianBlur(tempProcessingImg, tempProcessingImg, (blur_size * 2) + 1);

        //mask red hue
        inRange(tempProcessingImg, Scalar(0, 0, 0), Scalar(low_Hue, 255, max_value), tempMask1);
        inRange(tempProcessingImg, Scalar(high_Hue, 0, 0), Scalar(255, 255, max_value), tempMask2);
        bitwise_or(tempMask1, tempMask2, maskedFrame);

        //array of middle of white points
        int middles[maskedFrame.rows];
        for(int i = 0; i < maskedFrame.rows; i++)
        {
            int numWhite = 1;
            int avgPos = 0;
            const uchar* Mi = maskedFrame.ptr<uchar>(i);
            for(int j = 0; j < maskedFrame.cols; j++){
                if(Mi[j] > 0){
                    numWhite++;
                    avgPos += j;
                }
            }
            middles[i] = (int)avgPos/numWhite;
        }

        //iterate through array of middle white pixel locations
        int avgMiddle;   
        for (int i = 0; i < maskedFrame.rows; ++i)
        {
            avgMiddle += middles[i];
            //cout <<"row [" << i << "] = " << middles[i] << endl ;
        }
        avgMiddle /= maskedFrame.rows;


        cout << "avgMiddle = " << avgMiddle<< endl;

        if(avgMiddle > center + padding) {
            Movement = "right";
            cout << "move right\n";
        } else if(avgMiddle < center - padding) {
            Movement = "left";
            cout << "move left\n";
        } else {
            Movement = "straight";
            cout << "going straight enough\n";
        }

        /*
        Mat middlesLine(maskedFrame.rows, maskedFrame.cols, CV_8U, 0);
        for(int i = 0; i < middlesLine.rows; i++) {
            uchar* Mi = middlesLine.ptr<uchar>(i);
            for (int j = 0; j < middlesLine.cols; ++j)
            {
                if(Mi[j] == middles[i]) {
                    Mi[j] = 1;
                }
            }
        }
        */
        //middlesLine = np.zeros((maskedFrame.rows,maskedFrame.cols), np.uint8)

        //convert back to RGB before displaying
        //cvtColor(tempProcessingImg, maskedFrame, COLOR_HSV2RGB);
        //find contours and display
        Mat canny_output, canny_output_bwSRC, bwsrc;
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;

        //Canny(maskedFrame, canny_output, canny_thresh, canny_thresh * canny_ratio, 3 );
        //Mat hsv_channels[3];
        //cv::split(tempProcessingImg, hsv_channels );
        //equalizeHist( hsv_channels[2], bwsrc );
        //Canny(bwsrc, canny_output_bwSRC, canny_thresh, canny_thresh * canny_ratio, 3 );
        //findContours( canny_output_bwSRC, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        // ** display source and result **
        //show source image
        imshow("Video Capture",frame);
        //show masked image
        imshow("maskedFrame", maskedFrame);
        //imshow("middleLine", middlesLine);
        //imshow("tempMask1", tempMask1);
        //imshow("tempMask2", tempMask2);
        //imshow("canny", canny_output);

        //imshow("canny 2", canny_output_bwSRC);

        /// Draw contours
        /*Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
         {
           Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
           drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
         }*/

        /// Show in a window
       // namedWindow( "Contours", CV_WINDOW_NORMAL );
        //imshow( "Contours", drawing );

        //json stuff
        Json::Value root;
        root["Movement"] = Movement;
        cout << root << endl;
    }
    



	return 0;
}

void on_low_hue(int, void *)
{
    //low_Hue = min(high_Hue-1, low_Hue);
    setTrackbarPos("Low Hue","maskedFrame", low_Hue);

    //inRange(frame, Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),frame_threshold);
    //imshow("Object Detection", frame_threshold);
}

void on_high_hue(int, void *)
{
    //high_Hue = min(high_Hue-1, low_Hue);
    setTrackbarPos("High Hue","maskedFrame", high_Hue);

    //inRange(frame, Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),frame_threshold);
    //imshow("Object Detection", frame_threshold);
}

void on_max_value(int, void *)
{
    setTrackbarPos("Max Value","maskedFrame", max_value);
}


void on_blur_size(int, void *)
{
    //high_Hue = min(high_Hue-1, low_Hue);
    setTrackbarPos("Blur","maskedFrame", blur_size);

    //inRange(frame, Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),frame_threshold);
    //imshow("Object Detection", frame_threshold);
}

void on_canny_thresh(int, void *)
{
    setTrackbarPos("canny thresh","maskedFrame", canny_thresh);   
}

void on_canny_ratio(int, void *)
{
    setTrackbarPos("canny ratio","maskedFrame", canny_ratio);   
}