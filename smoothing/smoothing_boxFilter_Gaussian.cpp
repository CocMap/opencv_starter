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
	Mat input_box, input_Gaussian, input_bilateral, input_median;
	Mat output_box, output_Gaussian, output_bilateral, output_median;


	VideoCapture cap(0);
	if(!cap.isOpened()) {
		cout <<"Error loading camera" <<endl;
		return -1;}

	namedWindow("Original",CV_WINDOW_AUTOSIZE);
	namedWindow("Box Filter",CV_WINDOW_AUTOSIZE);
	namedWindow("Gaussian", CV_WINDOW_AUTOSIZE);
//	namedWindow("")

	for(; ; frameCount++) {
		cap >> frame;

//
//		GaussianBlur(input,output_Gaussian, Size(5,5),1.5,1.5);

		//box filter method
		frame.copyTo(input_box);

		boxFilter(input_box,output_box,-1 , Size(50,50), Point(-1,-1), BORDER_DEFAULT);

		//Gaussian filter
		frame.copyTo(input_Gaussian);

		GaussianBlur(input_Gaussian, output_Gaussian,Size(11,11), 11, 0, BORDER_DEFAULT);

		imshow("Original",frame);
		imshow("Box Filter",output_box);
		imshow("Gaussian",output_Gaussian);

		if(waitKey(20) >= 0) break;

	}
	return 0;

}
