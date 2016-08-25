#include <iostream>
#include <cv.h>   // headers
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;
using namespace std;

int main (int argc, char** argv){
	int frameCount;
	Mat frame;
	Mat preFrame;
	Mat output;
	int i = 0;

	VideoCapture cap(0);
	if(!cap.isOpened()){
		cout << "Error loading camera, please check your camera connection! "
				<<endl;											//check if the camera open successfully
		return -1;
	}

	namedWindow("Original", CV_WINDOW_AUTOSIZE);

	int brightness_slide = 100;
	createTrackbar("Brightness", "Original", &brightness_slide, 255);


	for (; ; frameCount++){
		cap >> frame;

		i = brightness_slide;		//the brightness i depends on the value on Track bar
		output = frame + Scalar(i,i,i);
		if(i < 100) {
			output = frame - Scalar(50-i,50-i,50-i);
		}

		else {
		output = frame + Scalar(i,i,i);
		}


		if(waitKey(20) >= 0) break;

		imshow("Original", output);

	}

	return 0;

}

