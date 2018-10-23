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
    if (argc < 2) {
    	cerr << "please specify file to process\n";
    }

    const string source = argv[1];


    if (!cap.open(source)) {
        cout << "Not Opened" << endl;
        return -1;
    }

	return 0;
}