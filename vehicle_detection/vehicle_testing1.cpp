#include <iostream>
#include <cv.h>   // headers
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include "angle.h"

using namespace cv;
using namespace std;

int main (int argc, char** argv){
	//declare variables for frame
	int frameCount;
	Mat frame;
	Mat grayFrame;
	Mat blurFrame;
	Mat Canny_output;


	//declare variables for Canny
	int const lowThres = 100;
	int ratio = 3;
	int highThres = ratio*lowThres;
	int kernel_size = 3;

	//open testing video, download from YOUTUBE
	//vehicel night: https://www.youtube.com/watch?v=I8mJLHOS2eU
	//daylight: https://www.youtube.com/watch?v=OA8w2tXNqMY
	char videoFileName[50] = ("testingVideo.mp4");
	VideoCapture cap(videoFileName);
	if(!cap.isOpened()){
		cout << "Error loading camera, please check your camera connection! "
				<<endl;											//check if the camera open successfully
		return -1;
	}

	//take frame from camera
	cap >> frame;


	//show cols and rows of the window(frame)
	int cols = frame.cols;
	int rows = frame.rows;
	cout <<"The number of window rows: " <<rows <<endl;
	cout <<"The number of window cols: " <<cols <<endl;


	//create windows
	namedWindow("Original", CV_WINDOW_NORMAL);
	namedWindow("Crop Canny_Working Window",CV_WINDOW_AUTOSIZE);
//	namedWindow("Contour",CV_WINDOW_AUTOSIZE);
	namedWindow("Hough Line", CV_WINDOW_AUTOSIZE);



	for (frameCount = 0; ; frameCount++) {
		cap >> frame;

		imshow("Original", frame);									//imshow the original video

		cvtColor(frame,grayFrame,CV_RGB2GRAY);						//make gray scale

		GaussianBlur(grayFrame,blurFrame,Size(3,3),7,7);			//filter the noise using GaussianBlur

		Canny(blurFrame,Canny_output,lowThres, highThres, kernel_size);	//Canny edges


		//crop the edge, just area using to detect the lane
		int x,y;

		for(y = 0; y < 450; y++){
			for(x = 0; x < frame.cols; x++){
				Canny_output.at<uchar>(y,x) = 0;			//canny ecrased top of the edges
			}
		}

		if (frameCount < 480) {
			for(y = 450; y < 500; y++){
				for(x = 800; x < cols; x++){
					Canny_output.at<uchar>(y,x) = 0;
				}
			}
		}

		Mat cropCanny(Canny_output);								//Canny cropped, input for Contour

//		//find contour
//		//http://opencvexamples.blogspot.com/2013/09/find-contour.html
//		vector<vector<Point> > contour;
//		vector<Vec4i> hierarchy;
//		findContours(cropCanny, contour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0,0));
//
//		//draw contour
//		Mat drawingContour = Mat::zeros(cropCanny.size() ,CV_8UC3);
//
//		for (int i = 0; i < contour.size(); i++) {
//			drawContours(drawingContour,contour,i,Scalar(0,128,255),2,8,hierarchy,0,Point());
//		}



		//Hough line
		Mat houghLine(grayFrame);
#if 0
		vector<Vec2f> lines;
		HoughLines(cropCanny, lines, 100, CV_PI/50, 100, 0, 0);
		for(size_t i = 0; i < lines.size(); i++) {
			float rho = lines[i][0], theta = lines[i][0];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = Round(x0 + 1000*(-b));
			pt1.y = Round(y0 + 1000*(a));
			pt2.x = Round(x0 - 1000*(-b));
			pt2.y = Round(y0 - 1000*(a));
			line(houghLine, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
		}
#else
		vector<Vec4i> lines;
		HoughLinesP(cropCanny, lines, 1, CV_PI/180, 100, 50, 50);
		for( size_t i = 0; i < lines.size() ; i++ ){
		    Vec4i l = lines[i];
		    line( houghLine, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
		}
#endif






		//imshow the window
		imshow("Original",frame);
		imshow("Crop Canny",cropCanny);
//		imshow("Found Contour", drawingContour);
		imshow("Hough Line",houghLine);


		if(waitKey(20) >= 0) break;


	}

	return 0;
}
