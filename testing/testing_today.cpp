#include <iostream>
#include <cv.h>   // headers
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <stdio.h>
#include <math.h>

using namespace cv;
using namespace std;

int main (int argc, char** argv){
	//declare variables for frame
	int frameCount;
	Mat frame;
	Mat grayFrame;
	Mat blurFrame;
	Mat Canny_output;

	int x,y;
//	int xTop, yTop;				//the first point, on the top of processing area
//	int xMu, yMu;				//the mutual point for 2 vector
//	int xBot, yBot;				//the last point in the, bottom right corner




	//declare variables for Canny
	int const lowThres = 100;
	int ratio = 3;
	int highThres = ratio*lowThres;
	int kernel_size = 3;

	//open testing video, download from YOUTUBE
	//vehicel night: https://www.youtube.com/watch?v=I8mJLHOS2eU
	//daylight: https://www.youtube.com/watch?v=OA8w2tXNqMY
	char videoFileName[30] = ("testingVideo.mp4");
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
	namedWindow("Rectangle",CV_WINDOW_AUTOSIZE);
	namedWindow("Hough Line",CV_WINDOW_AUTOSIZE);
	namedWindow("Contour_Hough Line", CV_WINDOW_AUTOSIZE);
	namedWindow("Result", CV_WINDOW_AUTOSIZE);



	for (frameCount = 0; ; frameCount++) {
		cap >> frame;

	//	imshow("Original", frame);									//imshow the original video

		cvtColor(frame,grayFrame,CV_RGB2GRAY);						//make gray scale

		//change the color from gray Scale
		for( x = 0; x < cols; x++) {
			for (y = 500; y < rows; y++){

				if(grayFrame.at<uchar>(y,x) > 170 && grayFrame.at<uchar>(y,x) < 255) {

					frame.at<Vec3b>(y,x) [0] = 0;
					frame.at<Vec3b>(y,x) [1] = 255;
					frame.at<Vec3b>(y,x) [2] = 0;
				}
			}
		}
		imshow("Original", frame);

		GaussianBlur(grayFrame,blurFrame,Size(3,3),7,7);			//filter the noise using GaussianBlur

		Canny(blurFrame,Canny_output,lowThres, highThres, kernel_size);	//Canny edges

		Mat copyCanny(Canny_output);







		//crop the edge, just area using to detect the lane


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
		for(size_t i = 0; i < lines.size() ; i++ ){
		    Vec4i l = lines[i];
		    line( houghLine, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
		}
#endif

		imshow("Hough Line", houghLine);

		//find contour on Crop Canny, only show the lines
		//http://opencvexamples.blogspot.com/2013/09/find-contour.html
		vector<vector<Point> > contour;
		vector<Vec4i> hierarchy;
		findContours(Canny_output, contour, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0,0));

		//Approximate contours to polygons and get bounding
				vector<vector<Point> > contour_poly (contour.size());
				vector<Rect>boundRect (contour.size());
				vector<Point2f>center (contour.size());
				vector<float>radius(contour.size());

				contour.size() == 300;

				for (int i = 0; i < contour.size(); i = i ++){
					approxPolyDP (Mat (contour[i]), contour_poly[i], 3, true);
					boundRect[i] = boundingRect(Mat(contour_poly[i]));
					minEnclosingCircle((Mat)contour_poly[i],center[i],radius[i]);
				}

				//draw polygonal contour + bonding rect
				Mat drawing = Mat::zeros(Canny_output.size(),CV_8UC3);
				for (int i = 0; i < contour.size(); i = i++){
					drawContours(drawing, contour_poly, i, Scalar(0,128,255),1,8,vector<Vec4i>(),0,Point());
					rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(0,128,255),2,8,0);
				}

				imshow("Rectangle",drawing);

//		//draw contour
//		Mat drawingContour = Mat::zeros(houghLine.size() ,CV_8UC3);
//
//		for (int i = 0; i < contour.size(); i++) {
//			drawContours(drawingContour,contour,i,Scalar(255,255,255),2,8,hierarchy,0,Point());
//		}
//
//		imshow("Contour_Hough Line",drawingContour);
//



//		//display in the process frame
//		for (y = 0; y < rows; y++) {
//			for (x = 0; x < cols; x++) {
//				if(drawingContour.at<uchar>(y,x) == 255) {
//
//					frame.at<Vec3b>(y,x)[0] = 0;
//					frame.at<Vec3b>(y,x)[1] = 0;
//					frame.at<Vec3b>(y,x)[2] = 255;
//				}
//			}
//		}







		//set angle

		//finding 2 vector - lane line and the last line (last row)


		//set up every single point
		//top point
//		yTop = 500;
//		for (int x = 0; x < frame.cols; x++) {
//			if(drawingContour.at<uchar>(yTop,x) == 255) {
//				xTop = x;
//			}
//		}
//
//		//mutual point
//		yMu = rows;
//		for (int x = 0; x < frame.cols; x++) {
//			if(drawingContour.at<uchar>(yMu,x) == 255) {
//				xMu = x;
//			}
//		}
//
//		//bottom point
//		xBot = cols;
//		yBot = rows;
//
//		//draw line
//		line(drawingContour, Point(xTop,yTop), Point(xMu,yMu),Scalar(0,0,255),3,8,0);
//		line(drawingContour, Point(xBot,yBot), Point(xMu,yMu),Scalar(0,128,255),3,8,0);


		//angle between 2 vector
//		double angleCurve;
//
//		angleCurve = cos((xTop*yTop+xMu*yMu+xBot*yBot)/
//				(sqrt(pow(xTop,2) + pow(xMu,2) + pow(xBot,2))*sqrt(pow(yTop,2) + pow(yMu,2) + pow(yBot,2))));
//
//		cout <<"The angle: " <<angleCurve <<endl;



		Mat displayBox = frame(cv::Rect(10, 10, 300, 400));
		Mat color(displayBox.size(), CV_8UC3, cv::Scalar(255,0,0));
		double alpha = 0.3;
		addWeighted(color, alpha, displayBox, 1.0 - alpha , 0.0, displayBox);
		rectangle(frame,Rect(10,10,300,400),Scalar(0,0,255),2,8,0);

		imshow("Result",frame);













		if(waitKey(20) >= 0) break;


	}

	return 0;
}
