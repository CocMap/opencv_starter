//	Track a moving object using motion features.
#include <vector>
#include <cv.h>
#include <opencv2/core/core.hpp>   // basic OpenCV structures
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;

int main(  int argc, char** argv )  {

  int frameCount = 0;

  VideoCapture cap(0); // open the camera
  if(!cap.isOpened()) {  // check if we succeeded
      return -1;
  }



  namedWindow("Raw Capture",CV_WINDOW_NORMAL);
  namedWindow("Difference Frame",CV_WINDOW_NORMAL);
  namedWindow("Gray Difference",CV_WINDOW_NORMAL);

  namedWindow("Second Camera", CV_WINDOW_NORMAL);
  Mat frame1; // frame from the second camera

  Mat frame, prevFrame, frameDiff, grayDiff;
  cap >> frame; // get a new frame
  int xCenter = frame.cols / 2;
  int yCenter = frame.rows / 2;
  int widthObject = frame.cols / 4;   // expected width of the object
  int diffValue;
  int x, y;
  int sum, sumX, sumY;

  for(frameCount = 0;  ; frameCount++) {

	cap >> frame1; // get a frame from the second camera
	imshow("Second Camera", frame1);

    frame.copyTo(prevFrame);
    cap >> frame;
    flip( frame,frame,1 );  // flip the image horizontally
    imshow("Raw Capture", frame);
    absdiff( frame,prevFrame,frameDiff );
    imshow("Difference Frame", frameDiff);
    cvtColor( frameDiff, grayDiff, CV_BGR2GRAY, 1);  // convert to gray scale
    imshow("Gray Difference", grayDiff);

    sum = 0;   // these sums will be used to compute the x and y center of mass
    sumX = 0;
    sumY = 0;
    for (y = yCenter-widthObject; y < yCenter+widthObject; y++) {
      for (x = xCenter-widthObject; x < xCenter+widthObject; x++) {
   	  if (y >= 0 && y < frame.rows && x >= 0 && x < frame.cols) {
          diffValue = grayDiff.at<unsigned char>(y,x);
   	    sum += diffValue;
   	    sumX += x * diffValue;
   	    sumY += y * diffValue;
   	  }
   	}
    }
    if (sum > 0) {  // update the center
      xCenter = ((int)(1.0*sumX/sum) + xCenter) / 2; // x and y center of mass,smoothed
      yCenter = ((int)(1.0*sumY/sum) + yCenter) / 2;
    }
    // put a red circle on the object
    circle( frameDiff, Point(xCenter,yCenter),widthObject/2, Scalar(0,0,255), 2, 8 );
    imshow("Difference Frame",frameDiff);
    if(waitKey(20) >= 0) break;
  }
  return 0;
}
