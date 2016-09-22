#include <iostream>
#include <cv.h>   // headers
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

//include time library
#include <stdio.h>
#include <time.h>

using namespace cv;
using namespace std;


int main (int argc, char** argv){
	int frameCount;
	Mat frame, preFrame, grayDiff, frameDiff;
	Mat grayFrame, blurFrame, Canny_output;


	//declare variable for motion

	double vehicle_speed_mps, vehicle_distance, vehicle_time;
	double vehicle_speed_kmph;


	//declare variables for Canny
	int const lowThres = 100;
	int ratio = 3;
	int highThres = ratio*lowThres;
	int kernel_size = 3;


	char videoFileName[50] = ("speedVehicle.MOV");
	VideoCapture cap(videoFileName);
	if(!cap.isOpened()){
		cout << "Error loading camera, please check your camera connection! "
				<<endl;											//check if the camera open successfully
		return -1;
	}

	cap >> frame;
	int cols = frame.cols;
	int rows = frame.rows;

	cout <<"Frame cols are: " <<cols <<endl;
	cout <<"Frame rows are: " <<rows <<endl <<endl;


	//create window
	namedWindow("Original", CV_WINDOW_NORMAL);
	namedWindow("Process", CV_WINDOW_NORMAL);

	for ( frameCount = 0; ; frameCount++) {
		frame.copyTo(preFrame);
		cap >> frame;
		flip(frame,frame,0);

		absdiff(frame,preFrame,frameDiff);
		cvtColor(frameDiff,grayDiff,CV_RGB2GRAY);

		GaussianBlur(grayDiff,blurFrame,Size(3,3),7,7);	 		//filter the noise using GaussianBlur
		Canny(blurFrame,Canny_output,lowThres, highThres, kernel_size);	//Canny edges

		int t1 = 0;
		int t2 = 0;
		double fps = cap.get(CV_CAP_PROP_FPS);		//fps = 59.9535ms

		for (int y = 0; y < rows; y++){
			if (Canny_output.at<uchar>(y,10) > 250) {
				t1 = frameCount;
				break;
				return 0;

			}
		}


		for (int y = 0; y < rows; y++){
			if (Canny_output.at<uchar>(y,cols - 10) > 250) {
				t2 = frameCount;
				break;
				return 0;
			}
		}

			vehicle_distance = 18;	//m
			vehicle_time = (t2 - t1)*(1/fps);	//s
			vehicle_speed_mps = vehicle_distance/vehicle_time;
			if(vehicle_time > 0) {
				cout <<"Distance: " <<vehicle_distance << " m" <<endl;
				cout <<"Time: " <<vehicle_time <<" s" <<endl;
				cout <<"Speed in m/s: " <<vehicle_speed_mps <<" m/s" <<endl;

				vehicle_speed_kmph = vehicle_speed_mps*3600/1000;
				cout <<"Speed in km/h: " <<vehicle_speed_kmph <<" km/h" <<endl;

			}

			putText(Canny_output,"Distance: 18 meters", Point (20,50),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,255),2,8);
			char nameA[100];
			sprintf(nameA,"Time: %.2f", vehicle_time);
			putText(Canny_output, nameA, Point (20,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,255),2,8);

			char nameB[100];
			sprintf(nameB,"Speed in m/s: %.2f", vehicle_speed_mps);
			putText(Canny_output, nameB, Point (20,150),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,255),2,8);

			char nameC[100];
			sprintf(nameC,"Speed in km/h: %.2f", vehicle_speed_kmph);
			putText(Canny_output, nameC, Point (20,200),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,255),2,8);


		imshow("Original",frame);
		imshow("Process", Canny_output);

		if(waitKey(20) >= 0) break;
	}

	return 0;
}
