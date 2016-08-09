//smoothing image
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

Mat inputImage = imread ("Korean_road.jpg");
Mat outputImage;
int MAX_KERNEL_LENGTH = 31;

int display_caption(char* caption);
int display_output(int delay);



int main(  int argc, char** argv ) {
	namedWindow("Original", CV_WINDOW_AUTOSIZE);							//open window
	namedWindow("Output Image", CV_WINDOW_AUTOSIZE);

	if(!inputImage.data) {cout <<"Error loading image" <<endl;return 0;} 	//check the image load or not

	inputImage.copyTo(outputImage);
	
	caption = ("Homogeneous Blur"); 

	if (display_caption("Homogeneous Blur") != 0) {return 0;}

	for (int i = 1; i < MAX_KERNEL_LENGTH; i=i+2) {
		blur(inputImage,outputImage,Size(i,i),Point(-1,-1));
		if(display_output(100)!=0){return 0;}







	waitKey(0);
	return(0);
//	GaussianBlur(image,outputImage,x )

//		if(waitKey(20) >= 0) break;
	}
    return 0;

}
