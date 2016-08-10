#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;
using namespace std;

int main () {
char imageFile[50] = "Korean_road.jpg";		//name image
Mat image = imread (imageFile);			//read input image

namedWindow ("Korean road Window");		//create image window named "Korean road Window"
imshow("Korean road Window", image);		//show the image on Window

waitKey(0);						//close Window until pressing any key "exit"
return (0);
}
