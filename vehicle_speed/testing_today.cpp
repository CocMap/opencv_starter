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


//	char videoFileName[50] = ("speedVehicle.MOV");
	VideoCapture cap(1);
	if(!cap.isOpened()){
		cout << "Error loading camera, please check your camera connection! "
				<<endl;											//check if the camera open successfully
		return -1;
	}

	vehicle_distance = 0.3;	//m

	cout <<"Distance: " <<vehicle_distance << " m" <<endl;

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

		absdiff(frame,preFrame,frameDiff);
		cvtColor(frameDiff,grayDiff,CV_RGB2GRAY);

		GaussianBlur(grayDiff,blurFrame,Size(3,3),7,7);	 		//filter the noise using GaussianBlur
		Canny(blurFrame,Canny_output,lowThres, highThres, kernel_size);	//Canny edges

		double t1 = 0;
		double t2 = 0;
		int fps = 30; //cap.get(CV_CAP_PROP_FPS);		//fps = 59.9535ms

		for (int y = 0; y < rows; y++){
			for(int x = 10; x < 12; x++){

				if (Canny_output.at<uchar>(y,x) > 250) {
					t1 = frameCount;
					cout <<"t1:" <<t1 <<endl;
					break;
					return 0;
				}
			}
		}


		for (int y = 0; y < rows; y++){
			for(int x = cols - 12; x < cols - 10; x++){
				if (Canny_output.at<uchar>(y,x) > 250) {
					t2 = frameCount;
					cout <<"t2: " <<t2 <<endl;
					break;
					return 0;
				}
			}
		}



			vehicle_time = (t2 - t1);
			//*(1/fps);	//s

			cout <<vehicle_time <<endl;
//
//
//			if(vehicle_time > 0.5) {
//
//				cout <<"Time: " <<vehicle_time <<" s" <<endl;
//
//				vehicle_speed_mps = vehicle_distance/vehicle_time;
//				cout <<"Speed in m/s: " <<vehicle_speed_mps <<" m/s" <<endl;
//
//				vehicle_speed_kmph = vehicle_speed_mps*3600/1000;
//				cout <<"Speed in km/h: " <<vehicle_speed_kmph <<" km/h" <<endl;
//			}
//
//
//			if (vehicle_time < 0) {
//				vehicle_time = abs(vehicle_time);
//
//				cout <<"Time: " <<vehicle_time <<" s" <<endl;
//
//				vehicle_speed_mps = vehicle_distance/vehicle_time;
//				cout <<"Speed in m/s: " <<vehicle_speed_mps <<" m/s" <<endl;
//
//				vehicle_speed_kmph = vehicle_speed_mps*3600/1000;
//				cout <<"Speed in km/h: " <<vehicle_speed_kmph <<" km/h" <<endl;
//			}
//
//
//
//
//



		imshow("Original",frame);
		imshow("Process", Canny_output);

		if(waitKey(20) >= 0) break;
	}

	return 0;
}
