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
	Mat grayFrame;
	Mat edges;
	Mat blurGrayFrame;

	int motion = 0;

	//open video - change the videoFileName
//	char videoFileName[50] = ("testingVideo.MOV");
//op	VideoCapture cap(videoFileName);

	//open default camera
	VideoCapture cap(1);

	if(!cap.isOpened()){return -1;}
	cap >> frame;

	cout <<"Frame columns: " <<frame.cols <<endl;			//look at frame cols and rows to DRAW
	cout <<"Frame rows: " <<frame.rows <<endl;

	int cols = frame.cols;
	int rows = frame.rows;

	namedWindow("Original",CV_WINDOW_AUTOSIZE);
	namedWindow("Gray Scale",CV_WINDOW_AUTOSIZE);
	namedWindow("Results",CV_WINDOW_AUTOSIZE);
	namedWindow("Canny Edge", CV_WINDOW_NORMAL);

	for (; ; frameCount++){
		cap >> frame;

		imshow("Original", frame);

		cvtColor(frame,grayFrame,CV_RGB2GRAY);				//make the frame to gray, take effect in gray scale;
		GaussianBlur(grayFrame, blurGrayFrame, Size(7,7),1.5,1.5);
		Canny (blurGrayFrame, edges, 0, 30, 3);
		imshow("Canny Edge", edges);

		motion = motion + grayFrame.at<uchar>(0,0);

		int x, y;											 //x run to cols, y run to rows
		for (x = (cols/2 - 100) ; x < (cols/2 + 100); x++) {
			for (y = 0 ; y < rows; y++){
				if (grayFrame.at<uchar>(y,x) < 70) {
				grayFrame.at<uchar>(y,x) = 0;

				frame.at<Vec3b>(y,x) [0] = 0;
				frame.at<Vec3b>(y,x) [1] = 255;
				frame.at<Vec3b>(y,x) [2] = 255;
				}
			}
		}

		for (x = (cols/2 + 100) ; x < cols; x++) {
			for (y = 100 ; y < 200; y++){
				if (grayFrame.at<uchar>(y,x) < 100) {
					grayFrame.at<uchar>(y,x) = 0;

					frame.at<Vec3b>(y,x) [0] = 255;
					frame.at<Vec3b>(y,x) [1] = 0;
					frame.at<Vec3b>(y,x) [2] = 0;

					line(frame,Point((cols/2 + 100),100),Point((cols - 10),100),Scalar(0,0,255),3,8,0);
				}
			}
		}

		imshow("Results",frame);
		imshow("Gray Scale",grayFrame);

		if(waitKey(20) >= 0) break;
	}
    return 0;

}
