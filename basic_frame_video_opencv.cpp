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

int main (int argc, char** argv){
	int frameCount;
	Mat frame;
	

	VideoCapture cap(0);
	if(!cap.isOpened()){
		cout << "Error loading camera, please check your camera connection! "
				<<endl;											//check if the camera open successfully
		return -1;
	}
	
	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	
	for ( ; ; frameCount++) {
		cap >> frame;
		
		
		
		imshow("Original",frame);
		
		
		
		if(waitKey(20) >= 0) break;		
	}
	
	return 0;
}
