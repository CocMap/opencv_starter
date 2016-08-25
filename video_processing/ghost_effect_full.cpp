// Ghost effect video
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
	Mat currentFrame;
	Mat preFrame;
	Mat grayFrame;
	Mat grayDiff;
	Mat frameDiff;
	int motion = 0;
	double alpha, beta;					//brightness of two frame
	Mat dstColor;
	Mat dstGray;

	VideoCapture cap(0);				//open default camera
	if(!cap.isOpened()){
		return -1;
	}

	cap >> frame;

	cout <<"Enter your brightness Image [-1.0 - 1.0]: " <<endl;
	cin >> beta;

	if(0.7 <= beta <= 1 ) {cout <<"Original" <<endl;}
	if(0.2 < beta < 0.7 && beta < 0) {cout <<"OMG GHOST!!!!!" <<endl <<"AHHHHHHHHH!!!!" <<endl;}
	if(beta == 0) {cout <<"You disappear :((" <<endl;}

	cap >> frame;

	namedWindow("Original",CV_WINDOW_AUTOSIZE);
	namedWindow("Ghost effect Gray",CV_WINDOW_AUTOSIZE);
	namedWindow("Ghost effect Color",CV_WINDOW_AUTOSIZE);

	for (; ; frameCount++){

		frame.copyTo(preFrame);			//take the original one, set as previousFrame, take effect

		cap >> currentFrame;			//take the current frame, compare with the previousFrane

		motion = motion + grayDiff.at<uchar>(0.0);

		alpha = (1.0 - beta);			//beta is brightness of current frame
		addWeighted(preFrame,alpha,currentFrame,beta,0.0,dstColor);		//decrease the brightness of the current Frame
		cvtColor(dstColor,dstGray,CV_RGB2GRAY);							//make gray scale

		imshow("Original",currentFrame);
		imshow("Ghost effect Gray",dstGray);
		imshow("Ghost effect Color",dstColor);

		if(waitKey(20) >= 0) break;
	}
    return 0;

}




