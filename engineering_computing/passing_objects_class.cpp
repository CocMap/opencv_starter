#include <iostream>
#include <cv.h>   // headers
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include "passing_object.h"

using namespace cv;
using namespace std;

int main (int argc, char** argv){
	PassingObject object1; // object1 is an object of type PassingObject
	int n = 1000;
	PassingObject object[n];			//create an array



	VideoCapture cap(0); // cap is an object of type VideoCapture



	Mat frame; // frame is an object of type Mat
	Mat frame1; // frame1 is an object of type Mat
	int green1, green;
	int threshold = 50;
	int count = 0;

	bool background = true;


	int frameCount = 0;
	namedWindow("Frame", CV_WINDOW_NORMAL);
	cap >> frame1;
	green1 = frame1.at<Vec3b>(frame1.rows/2,frame1.cols/2)[1]; // green value of center pixels

	namedWindow("Frame", CV_WINDOW_AUTOSIZE);
	namedWindow("Passing Object We Saw", CV_WINDOW_NORMAL);



		for (frameCount = 0; ; frameCount++) {
			cap >> frame;
			imshow("Frame", frame);
			green = frame1.at<Vec3b>(frame1.rows/2,frame1.cols/2)[1]; // green value of center pixels

		// if (green is different from green1) {
			if (abs(green - green1) > threshold) {
				if(background) {
					//object[count] where we store info of new object
					object[count].setPhoto(frame); // save the photo of the passing object
					object[count].setFrameNumber( frameCount ); // save the current frame count
					object[count].savePhotoToFile(count);
					imshow("Passing Object We Saw", object[count].getPhoto()); // show object1's photo
					object[count].printFrameNumber();

					count = count + 1; //this will point to the next object in the array
				//break; // exit the loop
				}

				background = false;

			} else {
				background = true;
			}

			if (waitKey(10) > 0) break;
		}

		//slide show display every object we detected and stored in the array
		for (int c = 0; c < count* 300; c++){
			imshow("Passing Object We Saw ", object[c/30].getPhoto());
		}

		waitKey(0);
		return 0;
}
