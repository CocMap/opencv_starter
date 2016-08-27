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
	Mat grayFrame;
	Mat blurFrame;
	Mat Canny_output;

	int const lowThres = 100;
	int ratio = 2;
	int highThres = ratio*lowThres;
	int kernel_size = 3;

	char videoFileName[50] = ("testingVideo.mp4");
	VideoCapture cap(videoFileName);
	if(!cap.isOpened()){
		cout << "Error loading camera, please check your camera connection! "
				<<endl;											//check if the camera open successfully
		return -1;
	}

	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	namedWindow("Gaussian",CV_WINDOW_AUTOSIZE);

	namedWindow("Found Contour",CV_WINDOW_AUTOSIZE);
	namedWindow("Drawing Contour",CV_WINDOW_AUTOSIZE);


	for ( ; ; frameCount++) {
		cap >> frame;
		flip(frame, frame, 1);

		GaussianBlur(frame,blurFrame,Size(3,3),7,7);
		Canny(blurFrame,Canny_output,lowThres, highThres, kernel_size);

		//find contour
		//http://opencvexamples.blogspot.com/2013/09/find-contour.html
		Mat inputContour;
		Canny_output.copyTo(inputContour);
		vector<vector<Point> > contour;
		vector<Vec4i> hierarchy;
		findContours(inputContour, contour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0,0));

		//draw contour
		Mat drawing = Mat::zeros(inputContour.size() ,CV_8UC3);

		for (int i = 0; i < contour.size(); i++) {
			drawContours(drawing,contour,i,Scalar(0,128,255),2,8,hierarchy,0,Point());
		}


		//hought







		imshow("Original",frame);
		imshow("Gaussian",Canny_output);

		imshow("Found Contour", inputContour);
		imshow("Drawing Contour",drawing);


		if(waitKey(20) >= 0) break;
	}

	return -1;
}
