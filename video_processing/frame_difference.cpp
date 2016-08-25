//Frame difference

#include <cv.h>   // headers
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;

int main(  int argc, char** argv ) {
	int frameCount;
	Mat frame;
	Mat currentFrame;
	Mat preFrame;
	Mat grayDiff;
	Mat frameDiff;

	//create ROI (region of interest)
//	int xRoi = 0;			,	//position of ROI in x-axis
//	int yRoi = 0;				//position of ROI in y-axis
//	int hRoi = 0;					//height of ROI
//	int wRoi = 0;					//width of ROI
	int motion = 0;

	VideoCapture cap(0);
	if (!cap.isOpened()) {			//check the if the video is opened successful
		return -1;
	}

	cap >> frame;					//take one frame from camera

	namedWindow("Camera", CV_WINDOW_AUTOSIZE);					//open a window, default camera
	namedWindow("Frame Difference", CV_WINDOW_AUTOSIZE);		//open frame difference (RGB)
	namedWindow("Gray Difference", CV_WINDOW_AUTOSIZE);			//open gray difference (gray-scale)

	for (frameCount = 0; ; frameCount++) {
		frame.copyTo(preFrame);									//copy a frame, set as previous frame

		cap >> frame;											//take one frame, set as current frame
		absdiff(frame,preFrame,frameDiff);						//make the difference between 2 frames, frameDiff = RGB diff
		cvtColor(frameDiff, grayDiff, CV_RGB2GRAY,1);			//change the difference to gray scale

		motion = motion + grayDiff.at<unsigned char>(0,0);

		imshow("Camera", frame);
		imshow("Gray Difference",grayDiff);
		imshow("Frame Difference",frameDiff);

		if (waitKey(20) >= 0) break;
	}
	flip(frame,grayDiff,1);

    return 0;

}




