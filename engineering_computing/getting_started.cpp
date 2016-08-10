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

 Mat image; //an image, containing 2 dimensional data

 int xCenter = 100;
 int yCenter = 100;
 int radius = 50;
 int deltaX = 5;
 int deltaY = 4;

 char name[100]; // a string which is an array of characters
 printf("What is your name? \n");
 gets(name);
 printf("Hello %s and Welcome to Engineering Computing", name, "\n");

 VideoCapture cap(0); // open the default camera
 if(!cap.isOpened()) {  // check if we succeeded
     return -1;
 }

 namedWindow("Original Image Sequence",CV_WINDOW_AUTOSIZE);

 for(;; frameCount++) {

	  if (frameCount % 100 == 0) //the condition if the remainder of frameCount/100 is zero
	  {
		  printf("frameCount = %d \n", frameCount);
	  }

     Mat frame; // create a mat object called frame (an image)
     cap >> frame; // get a new frame from camera

     flip(frame,frame,1);

    //put Text according to (x,y) = (30,30)
     putText(frame, name, Point(30,30), FONT_HERSHEY_PLAIN, 2, Scalar (0, 0, 255)); // put the name on camera

     //draw a circle and it can move
     circle(frame, Point (xCenter, yCenter), radius, Scalar(255, 0, 255)); // draw a circle
     rectangle(frame, Rect(15,15,60,80), Scalar(0,255,0)); // draw a rectangle
     xCenter = xCenter + deltaX; //add deltaX to xCenter
     yCenter = yCenter + deltaY; //add deltaY to yCenter


     if(xCenter > frame.cols-radius || xCenter <0+radius) // x is at the edge
     {
    	 deltaX = -deltaX;
     }

     if (yCenter > frame.rows-radius || yCenter < 0 + radius)
     {
    	 deltaY = -deltaY;
     }

     imshow("Original Image Sequence", frame); //display the frame
     if(waitKey(20) >= 0) break; //if the user presses a key, then exit
 }
 // the camera will be de-initialized automatically in VideoCapture destructor

 printf("frameCount = %d \n", frameCount);
 return 0;
 }


