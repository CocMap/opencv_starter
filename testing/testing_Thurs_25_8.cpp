#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;
using namespace std;

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

Mat inputImg;
Mat outputImg;

int display_caption(char* caption){
	outputImg = Mat(inputImg.size(), inputImg.type());
	putText(outputImg, caption, Point(inputImg.cols/4, inputImg.rows/2),CV_FONT_HERSHEY_COMPLEX,1,Scalar(255,0,255));

	imshow("Results",outputImg);

	if(waitKey(20) >= 0){return -1;};
	return 0;
}

int display_output(int delay) {
	imshow("Results",outputImg);

	if(waitKey(20) >= 0){return -1;};
	return 0;
}

int main () {
	Mat image = imread ("doraemonNew.jpg");				//read input image

	namedWindow("Results",CV_WINDOW_AUTOSIZE);

	inputImg.copyTo(outputImg);
	if(display_output(DELAY_CAPTION) != 0) {return 0;}

	if(display_caption("Homogeneous Blur") != 0) {return 0;}

	cout << "size: Height = " <<image.rows <<endl;
	cout <<", Width = " <<image.cols <<endl;		//read the Size of the image

	if (!image.data) {cout << "Error loading image" <<endl;}

	namedWindow("Original", CV_WINDOW_NORMAL);			//create image window named "Korean road Window"
	imshow("Original", image);			//show the image on Window

	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2) {
		blur (inputImg, outputImg, Size(i,i), Point(-1,-1));
		if (display_output(DELAY_BLUR)!=0) {return 0;}
	}



	waitKey(0);
	return(0);

}

