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
	char imageFile[50] = "Korean_road.jpg";			//name image
	Mat image = imread (imageFile);				//read input image

	cout << "size: Height = " <<image.size() .height 
	<<", Width = " <<image.size() .width <<endl;		//read the Size of the image

	if (!image.data) {						//check if any errors reading image
		cout << "Read image successfully" <<endl;
		}

	namedWindow("Korean road Window");			//create image window named "Korean road Window"
	imshow("Korean road Window", image);			//show the image on Window

	Mat result;							//flip [Horizontal(1), Vertical(0), Negative (-1)]
	flip(image, result, -1);

	namedWindow("Output image");				//create window and show image1
	imshow("Output image", result);

	waitKey(0);
	return(0);
}
