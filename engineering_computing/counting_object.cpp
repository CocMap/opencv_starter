// Counting of passing objects
#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;

int main(  int argc, char** argv ) {
  int frameCount = 0;  // current number of frames.  Set its value to 0
  Mat frame;  // create a Mat object called frame (an image)
  Mat previousFrame;  // contain the "previous" frame
  Mat frameDiff;   // the difference between the current and previous frame
  Mat grayDiff;  // the gray scale of frameDiff

  char videoFileName[50] = "rmit-nvl-2.mp4";
  VideoCapture cap(videoFileName); // open the default camera
 
  if(!cap.isOpened()) {  // check if we succeeded
          return -1; // If the camera did not open, exit the program
       }
      cap >> frame;   // get one frame from the camera
     // make the current frame previous

  int motion=0;
  int x,y;
  int xRoi = 300; // top-left x
  int yRoi = 300; // top-left y
  int widthRoi = 150;  // width of the roi
  int heightRoi = 150;  // height
  int previousMotion;
  int count=0; // holds the number of object we count
  float threshold = 1.0;

  float sum = 0.0;
  float averageMotion;


  namedWindow("Camera",CV_WINDOW_NORMAL); // Create a new window
  namedWindow("Frame Difference", CV_WINDOW_NORMAL);
  namedWindow("Gray Difference", CV_WINDOW_NORMAL);

  for(frameCount = 0;   ; frameCount++) {  // Add 1 to frameCount every iteration
     frame.copyTo( previousFrame ); // before reading from the camera,
     if (cap.read(frame) == NULL){
    	 cap.open(videoFileName);
    	 cap >> frame;
     }

     cap >> frame; // get a new frame from camera
     flip(frame,frame,1);
     rectangle(frame,Rect(xRoi,yRoi,widthRoi,heightRoi),Scalar(0,0,255),1);
     imshow("Camera", frame);  // display the frame
     // compute the absolute difference between frame and previousFrame
     // and store it in frameDiff
     absdiff( frame,previousFrame,frameDiff );
     imshow("Frame Difference", frameDiff);

     cvtColor( frameDiff,grayDiff, CV_BGR2GRAY,1 ); // let's make it gray scale
     imshow("Gray Difference",grayDiff);
     previousMotion = motion; // save the motion as previous, before recomputing
     motion = 0;  // the sum is 0
     for (x = xRoi; x < xRoi+widthRoi; x++) {
        for (y = yRoi; y < yRoi+heightRoi; y++) {
             // take the value at that x,y location and add to motion
            motion = motion + grayDiff.at<unsigned char>(y,x);
        }
     }

     sum = sum + motion; //
     averageMotion = sum / frameCount; //running average

     // if previousMotion is below the Threshold and current is above the threshold then
     // we had a new object
     if (previousMotion <= averageMotion * threshold && motion > averageMotion * threshold){
        count = count + 1;
        // Visualize the count increase on the image using a different colored using different color
        rectangle(frame,Rect(xRoi,yRoi,widthRoi,heightRoi), Scalar(255,255,0),8);
        imshow("Camera",frame);
     }

     printf("frameCount = %d Motion = %8d averageMotion = %2.0f Count = %4d \n",
    		 frameCount, motion, averageMotion, count); // %8d means an integer occupying 8 characters
     if (waitKey(20) >= 0) break;   // if the user presses a key, then exit
  }
  return 0;
}
