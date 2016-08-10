#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;

int main(  int argc, char** argv )
{
 int frameCount = 0;//current number of frame
 Mat frame; //an image, containing 2 dimensional data
 Mat previousFrame; // contain the previous frame
 Mat frameDiff; //difference between current and previous frame
 Mat grayDiff; // the gray scale of frameDiff

 VideoCapture cap(0); // open the default camera
 if(!cap.isOpened()) {  // check if we succeeded
     return -1;
 }

 cap >> previousFrame;// get one frame from the camera
 cap >> frame;//get another frame

 int motion;
 int x,y;
 int xRoi = 200; //top-left x
 int yRoi = 150; //top-left y
 int widthRoi = 200;// width of the roi
 int heightRoi = 200; // height of the roi

 namedWindow("Original Image Sequence",CV_WINDOW_AUTOSIZE);
 namedWindow("Difference");
 namedWindow("GrayDifference");

 for(;; frameCount++) {
	 frame.copyTo(previousFrame); // before reading from the camera

	  if (frameCount % 100 == 0) //the condition if the remainder of frameCount/100 is zero
	  {
		  printf("frameCount = %d \n", frameCount);
	  }

     Mat frame; // create a mat object called frame (an image)
     cap >> frame; // get a new frame from camera
     flip(frame,frame,1);
     imshow("Camera", frame); //display the frame

     absdiff(frame, previousFrame, frameDiff); //compute the absolute difference between frame
     //and previousFrame and store it in frameDiff
     imshow("Difference", frameDiff); //display the frame

     cvtColor(frameDiff, grayDiff, CV_BGR2GRAY, 1);
     imshow("Gray Difference", grayDiff);
     //compute the motion(sum of grayDiff inside the roi)
     // visualize the region of interest (roi)
     rectangle(frame, Rect(xRoi,yRoi,widthRoi,heightRoi),Scalar(0,255,255));

     motion = 0; //the sum is 0
     for(x = xRoi; x < xRoi + widthRoi; x++){
    	 for (y = yRoi; y < yRoi + widthRoi; y++){
    		 //take the value at that x,y location and add to the motion
    		 motion = motion + grayDiff.at<unsigned char>(y,x);
    	 }
     }
     if (frameCount % 3 == 0){
    	 printf("Motion %d\n", motion);
     }
     if (motion > 300000){
    	 rectangle(frameDiff, Rect(xRoi,yRoi,widthRoi,heightRoi),Scalar(0,0,255),4);
    	 imshow("Difference", frameDiff);
     }
    if(waitKey(20) >= 0) break; //if the user presses a key, then exit
 }
 // the camera will be de-initialized automatically in VideoCapture destructor

 printf("frameCount = %d \n", frameCount);
 return 0;
 }






	
