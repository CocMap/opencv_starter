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

int main(int argc, char** argv) {
	int frameCount;
	Mat frame, preFrame, grayDiff, frameDiff;
	Mat frameDetect;


	//global variables
	int motion = 0;
//	float distance;
//	double speed;
//	double t1;
//	double t2;
//	double deltaTime = t2 - t1;


	char videoFileName[50] = ("speedVehicle.MOV");
	VideoCapture cap (videoFileName);
	if(!cap.isOpened()) {
		cout << "Error loading video file, please check the source file!" <<endl;
		return -1;
	}

	cap >> frame;

	int cols = frame.cols;
	int rows = frame.rows;

	//show the size of the frame
	cout <<"Frame column is: " <<cols <<endl;
	cout <<"Frame rows is: " <<rows <<endl;


	//create the window
	namedWindow("Original", CV_WINDOW_NORMAL);
	namedWindow("Gray Difference", CV_WINDOW_NORMAL);


	for( ; ; frameCount++) {
		frame.copyTo(preFrame);									//copy a frame, set as previous frame
		cap >> frame;											//take one frame, set as current frame
		flip(frame,frame,0);
		imshow("Original",frame);

		absdiff(frame,preFrame,frameDiff);						//make the difference between 2 frames, frameDiff = RGB diff
		cvtColor(frameDiff, grayDiff, CV_RGB2GRAY);				//change the difference to gray scale

		imshow("Gray Difference",grayDiff);

		motion = motion + frame.at<uchar>(0,0); 				//detect through motion
//
//		for(int x = 0; x < cols; x++){
//			for(int y = 0; y < rows; y++) {
//				motion = motion + frame.at<uchar>(y,x);
//
//				rectangle(frame,Rect(x,y,100,200), Sca;r)
//
//
//			}
//		}
//






		if(waitKey(20) >= 0) break;
	}

	return 0;

}
