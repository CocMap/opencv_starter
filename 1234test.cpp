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
	Mat frame, input, output;
	Mat detectedEdge;
	Mat result;
	Mat grayFrame;

	int const lowThres = 100;
	int ratio = 2;
	int kernel_size = 5;
	int highThres = lowThres*ratio;


	VideoCapture cap(0);
	if(!cap.isOpened()) {
		cout <<"Error loading camera" <<endl;
		return -1;}

	namedWindow("Original",CV_WINDOW_AUTOSIZE);
	namedWindow("Box Filter",CV_WINDOW_AUTOSIZE);
	//namedWindow("Gaussian", CV_WINDOW_AUTOSIZE);

	for(; ; frameCount++){
		cap >> input;

		//create a matrix of same type and size of input
		output.create( input.size(), input.type());

		//transfer to gray image
		cvtColor(input,grayFrame,CV_RGB2GRAY);

		//reduce noise
//		boxFilter(grayFrame, detectedEdge, -1, Size(10,10), Point(-1,-1), BORDER_DEFAULT);

//		blur(grayFrame,detectedEdge, Size(15,15));
		GaussianBlur(grayFrame,detectedEdge,)

		//Canny detectiony
		Canny(detectedEdge, detectedEdge, lowThres, highThres, kernel_size);

		output = Scalar::all(0);

		input.copyTo(output, detectedEdge);

		imshow("Original", input);
		imshow("Box Filter", output);

		if(waitKey(20) >= 0) break;

	}

	return 0;

}
