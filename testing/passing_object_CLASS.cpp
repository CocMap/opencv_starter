#include <iostream>
#include <cv.h>   // headers
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>
#include "passing_object.h"

using namespace cv;
using namespace std;

int main (int argc, char** argv){
	PassingObject object1;


	int frameCount;
	Mat frame;
	Mat frame1;					//the first frame
	int green, green1;
	int threshold = 10;

//	char videoFileName[50] = ("testingVideo.mp4");
		VideoCapture cap(0);
		if(!cap.isOpened()){
			cout << "Error loading camera, please check your camera connection! "
					<<endl;											//check if the camera open successfully
			return -1;
		}


		cap >> frame1;
		green1 = frame1.at<Vec3b>(frame1.rows/2,frame1.cols/2)[1];		//green value of center pixel


		for ( ; ; frameCount++) {
				cap >> frame;

				imshow("Frame", frame);

				green = frame1.at<Vec3b>(frame.rows/2,frame.cols/2)[1];

				if(abs(green - green1) > threshold) {
					object1.setPhoto(frame);	//save the photo of the passing object
					object1.setFrameNumber(frameCount);
					break;						//exit the loop
				}
				
				


				if(waitKey(20) >= 0) break;
			}
		namedWindow("Passing Object We Saw", CV_WINDOW_AUTOSIZE);
		imshow("Passing Object We Saw", object1.getPhoto());
		object1.printFrameNumber();

		waitKey(0);

		return -1;
	}


