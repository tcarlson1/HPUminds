#include <iostream>
#include <stdlib.h>
#include <string>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv/cv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
	VideoCapture cap;
    Mat frame, maskedFrame, tempProcessingImg, tempMask1, tempMask2;
    int blur_size = 9;
    int low_Hue;
    int high_Hue;
    if (argc < 2) {
    	cerr << "please specify file to process\n";
    }

    const string source = argv[1];


    if (!cap.open(source)) {
        cout << "Not Opened" << endl;
        return -1;
    }

    namedWindow("Video Capture", WINDOW_NORMAL);
    createTrackbar("Low R","Object Detection", &low_Hue, 255, );

    //loop through video file
    while((char)waitKey(1)!='q'){
        cap >> frame;

        //break if at the end of the video file
        if(frame.empty())
            break;

        //rotate the image
        transpose(frame, frame);
        flip(frame, frame, 1);
        resize(frame, frame, Size(225,400));


        // ** mask out the path **
        //convert to hsv
        cvtColor(frame, tempProcessingImg, COLOR_RGB2HSV);
        //medianBlur(tempProcessingImg, tempProcessingImg, (blur_size * 2) + 1);

        //mask red hue
        inRange(tempProcessingImg, Scalar(0, 0, 0), Scalar(20, 255, 255), tempMask1);
        inRange(tempProcessingImg, Scalar(159, 0, 0), Scalar(179, 255, 255), tempMask2);
        bitwise_or(tempMask1, tempMask2, maskedFrame);

        //convert back to RGB before displaying
        //cvtColor(tempProcessingImg, maskedFrame, COLOR_HSV2RGB);

        // ** display source and result **
        //show source image
        imshow("Video Capture",frame);
        //show masked image
        imshow("masked", maskedFrame);
        imshow("tempMask1", tempMask1);
        imshow("tempMask2", tempMask2);
    }
	return 0;
}

void on_low_hue(int, void *)
{
    low_Hue = min(high_r-1, low_r);
    setTrackbarPos("Low R","Object Detection", low_Hue);

    //inRange(frame, Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),frame_threshold);
    //imshow("Object Detection", frame_threshold);

}

void on_high_hue(int, void *)
{
    high_Hue = min(high_r-1, low_r);
    setTrackbarPos("Low R","Object Detection", low_Hue);

    //inRange(frame, Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),frame_threshold);
    //imshow("Object Detection", frame_threshold);

}