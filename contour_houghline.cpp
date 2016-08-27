#include <iostream>
#include <cv.h>   // headers
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>

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
	cap >> frame;

	cout <<"The number of window rows: " <<frame.rows <<endl;
	cout <<"The number of window cols: " <<frame.cols <<endl;

	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	namedWindow("Gaussian",CV_WINDOW_AUTOSIZE);

	namedWindow("Found Contour",CV_WINDOW_AUTOSIZE);
	namedWindow("Drawing Contour",CV_WINDOW_AUTOSIZE);
	namedWindow("Result", CV_WINDOW_AUTOSIZE);


	for ( ; ; frameCount++) {
		cap >> frame;
		flip(frame, frame, 1);

		cvtColor(frame,grayFrame,CV_RGB2GRAY);

		GaussianBlur(grayFrame,blurFrame,Size(3,3),7,7);
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


#if 0
		vector<Vec2f> lines;
		HoughLines(Canny_output, lines, 100, CV_PI/50, 100, 0, 0);
		for(size_t i = 0; i < lines.size(); i++) {
			float rho = lines[i][0], theta = lines[i][0];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000*(-b));
			pt1.y = cvRound(y0 + 1000*(a));
			pt2.x = cvRound(x0 - 1000*(-b));
			pt2.y = cvRound(y0 - 1000*(a));
			line(grayFrame, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
		}
#else
		vector<Vec4i> lines;
		HoughLinesP(Canny_output, lines, 1, CV_PI/180, 100, 50, 50);
		for( size_t i = 0; i < lines.size() ; i++ ){
		    Vec4i l = lines[i];
		    line( grayFrame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
		}
#endif

		imshow("Original",frame);
		imshow("Gaussian",Canny_output);

		imshow("Found Contour", inputContour);
		imshow("Drawing Contour",drawing);
		imshow("Result", grayFrame);


		if(waitKey(20) >= 0) break;
	}

	return -1;
}
