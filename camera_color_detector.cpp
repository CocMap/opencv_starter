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

int main(  int argc, char** argv ) {
	int frameCount;
	Mat frame;
	Mat preFrame;
	Mat grayFrame;
	Mat frameDiff;
	Mat preGrayFrame;
	int motion = 0;


	char videoFileName[50] = ("testingVideo.MOV");

	VideoCapture cap(videoFileName);				//open default camera
	if(!cap.isOpened()){return -1;}
	cap >> frame;

	cout <<"Frame columns: " <<frame.cols <<endl;			//look at frame cols and rows to DRAW
	cout <<"Frame rows: " <<frame.rows <<endl;

	int cols = frame.cols;
	int rows = frame.rows;

	namedWindow("Original",CV_WINDOW_NORMAL);
	namedWindow("Gray Scale",CV_WINDOW_NORMAL);

	for (; ; frameCount++){
		cap >> frame;


		cvtColor(frame,grayFrame,CV_RGB2GRAY);				//make the frame to gray, take effect in gray scale;

		motion = motion + grayFrame.at<uchar>(0,0);

		int x, y;											 //x run to cols, y run to rows
		for (x = 0 ; x < cols; x++) {
			for (y = 0 ; y < rows; y++){
				if (grayFrame.at<uchar>(y,x) < 80) {
				grayFrame.at<uchar>(y,x) = 0;

				frame.at<Vec3b>(y,x) [0] = 0;
				frame.at<Vec3b>(y,x) [1] = 255;
				frame.at<Vec3b>(y,x) [2] = 255;
				}
			}
		}

		imshow("Original",frame);
		imshow("Gray Scale",grayFrame);

		if(waitKey(20) >= 0) break;
	}
    return 0;

}
