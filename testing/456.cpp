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

	int cols = frame.cols;
	int rows = frame.rows;


//	namedWindow("Original", CV_WINDOW_AUTOSIZE);
//	namedWindow("Canny",CV_WINDOW_AUTOSIZE);
//
//	namedWindow("Found Contour",CV_WINDOW_AUTOSIZE);
	namedWindow("Drawing Contour",CV_WINDOW_AUTOSIZE);
//	namedWindow("Result", CV_WINDOW_AUTOSIZE);
	namedWindow("Gray",CV_WINDOW_NORMAL);
//
//	namedWindow("Result_Canny",CV_WINDOW_AUTOSIZE);
	namedWindow("Result_gray",CV_WINDOW_NORMAL);

	//track bar for Canny
	int threshold_slide = 100;
	createTrackbar("Canny Threshold", "Canny", &threshold_slide, 500);


	for ( ; ; frameCount++) {
		cap >> frame;


//		imshow("Original", frame);

		cvtColor(frame,grayFrame,CV_RGB2GRAY);

		GaussianBlur(grayFrame,blurFrame,Size(1,1),7,7);

		int lowThres;
		lowThres = threshold_slide;
		int ratio = 3;
		int highThres = lowThres*ratio;
		Canny(blurFrame, Canny_output, lowThres, highThres, kernel_size,3);

//		//change the color of Canny
		int x, y;											 //x run to cols, y run to rows
//		for (x = 0 ; x < cols; x++) {
//			for (y = 500 ; y < rows; y++){
//				if (Canny_output.at<uchar>(y,x) == 255) {
//
//					frame.at<Vec3b>(y,x) [0] = 0;
//					frame.at<Vec3b>(y,x) [1] = 0;
//					frame.at<Vec3b>(y,x) [2] = 255;
//				}
//			}
//		}

//		imshow("Result_Canny",frame);

		//change the colof from gray Scale
		for(x = 0; x < cols; x++) {
			for (y = 500; y < rows; y++){

				if(grayFrame.at<uchar>(y,x) > 180 && grayFrame.at<uchar>(y,x) < 255) {

					frame.at<Vec3b>(y,x) [0] = 0;
					frame.at<Vec3b>(y,x) [1] = 255;
					frame.at<Vec3b>(y,x) [2] = 0;
				}
			}
		}


		//contours + bounding
		vector<vector<Point> > contour;
		vector<Vec4i> hierarchy;

		findContours(Canny_output, contour, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

		//Approximate contours to polygons and get bounding
		vector<vector<Point> > contour_poly (contour.size());
		vector<Rect>boundRect (contour.size());
		vector<Point2f>center (contour.size());
		vector<float>radius(contour.size());

		contour.size() == 300;

		for (int i = 200; i < contour.size(); i = i + 100){
			approxPolyDP (Mat (contour[i]), contour_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contour_poly[i]));
			minEnclosingCircle((Mat)contour_poly[i],center[i],radius[i]);
		}

		//draw polygonal contour + bonding rect
		Mat drawing = Mat::zeros(Canny_output.size(),CV_8UC3);
		for (int i = 200; i < contour.size(); i = i + 100){
			drawContours(drawing, contour_poly, i, Scalar(0,128,255),1,8,vector<Vec4i>(),0,Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(0,128,255),2,8,0);
		}

		cout <<"Contour size: " <<contour.size() <<endl;






		//car next
		for (x = 600 ; x < 700; x++) {
			for(y = 490; y < 500; y++) {
				if (grayFrame.at<uchar>(y,x) < 50) {
					line(frame,Point(500,500),Point(1000,500),Scalar(0,0,0),3,8,0);
					putText(frame, "Warning", Point(550,490),FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,255),5,8);

				}
			}
		}

		//car far
		for (x = 800 ; x < 850; x++) {
			for(y = 410; y < 500; y++) {
				if (grayFrame.at<uchar>(y,x) < 50) {
					grayFrame.at<uchar>(y,x) = 255;

					rectangle(frame, Rect(700,405,100,50), Scalar(0,0,255),5,8,0);

						}
					}
				}





		//imshow
		imshow("Gray", grayFrame);
		imshow("Result_gray",frame);
		imshow("Drawing Contour", drawing);


		if(waitKey(20) >= 0) break;
	}

	return -1;
}

