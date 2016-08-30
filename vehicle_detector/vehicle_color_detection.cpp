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

		namedWindow("Gray",CV_WINDOW_AUTOSIZE);
		namedWindow("Result",CV_WINDOW_AUTOSIZE);


		for ( ; ; frameCount++) {
				cap >> frame;

				cvtColor(frame,grayFrame,CV_RGB2GRAY);

				GaussianBlur(grayFrame,blurFrame,Size(1,1),7,7);

				int const lowThres = 100;
				int ratio = 3;
				int highThres = lowThres*ratio;
				Canny(blurFrame, Canny_output, lowThres, highThres, kernel_size,3);

				int x, y;

				//change the colof from gray Scale
				for(x = 0; x < cols; x++) {
					for (y = 500; y < rows; y++){

						if(grayFrame.at<uchar>(y,x) > 170 && grayFrame.at<uchar>(y,x) < 255) {

							frame.at<Vec3b>(y,x) [0] = 0;
							frame.at<Vec3b>(y,x) [1] = 255;
							frame.at<Vec3b>(y,x) [2] = 0;
						}
					}
				}


				//car next
				for (x = 600 ; x < 700; x++) {
					for(y = 490; y < 500; y++) {
						if (grayFrame.at<uchar>(y,x) < 20) {
							line(frame,Point(500,500),Point(1000,500),Scalar(0,0,0),3,8,0);
							putText(frame, "Warning", Point(550,490),FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,255),5,8);

						}
					}
				}


				//car left red
				for(x = 650; x < 700; x++){
				y = 420;
					if(grayFrame.at<uchar>(y,x) < 30) {
						rectangle(frame, Rect(550,420,100,50), Scalar(0,0,255),5,8,0);
					}
				}


				//car right blue
				for(x = 680; x < 750; x++){
				y = 420;
					if(grayFrame.at<uchar>(y,x) < 30) {
						rectangle(frame, Rect(680,420,100,50), Scalar(255,0,0),5,8,0);
					}
				}



				//exchange lane

//				for(x = 600; x < rows; x++){
//					for(y = 500; y < 600; y++){
//						grayFrame.at<uchar>(x,y) = 255;				//area of detect lane
//						if (frame.at<uchar>(x,y) == 255){
//							putText(frame, "Exchange lane", Point(100,100),FONT_HERSHEY_SIMPLEX, 1, Scalar(0,128,255),5,8);
//						}
//					}
//				}

				//imshow
				imshow("Gray", grayFrame);
				imshow("Result",frame);

				if(waitKey(20) >= 0) break;
			}

		return 0;
	}
