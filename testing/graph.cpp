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

//Function take an input frame
Mat invertFrame(Mat inputFrame) {
	Mat tempFrame; //a local object

	inputFrame.copyTo(tempFrame);			//copy input to temporary frame
	flip(tempFrame,tempFrame,1);			//do some processing
	return tempFrame;
}

Mat drawGraph(Mat graphFrame, int value, int cols) {
	Mat tempFrame;

	graphFrame.copyTo(tempFrame);
	flip(tempFrame,tempFrame,0);


	rectangle(tempFrame,Rect(cols,0,2,graphFrame.cols),Scalar(0,0,0),2); //erase teh column before drawing


	rectangle(tempFrame,Rect(tempFrame.cols - cols,0,2,value),Scalar(rand()%256,rand()%256,rand()%256),2);
	flip(tempFrame,tempFrame,0);




	return tempFrame;

}

int main(  int argc, char** argv ) {
	VideoCapture cap (0);
	Mat frame;
	int frameCount;
	Mat graph;


	 namedWindow("Frame",CV_WINDOW_AUTOSIZE);
	 namedWindow("Inverted Frame", CV_WINDOW_AUTOSIZE);
	 namedWindow("Graph", CV_WINDOW_AUTOSIZE);

	 cap >> graph;

	for(frameCount = 0; ; frameCount++){
		cap >> frame;

		circle(frame, Point(frame.cols/2,frame.rows/2),5,Scalar(0,127,255),3);

		imshow("Frame",frame);
		imshow("Inverted Frame",invertFrame(frame));

		//get the blue value of the frame center pixel
		//and draw it on the graph
		graph = drawGraph(graph, frame.at<uchar>(),frameCount % frame.cols);

		imshow("Graph",graph);

		if(waitKey(20) >= 0) break;
	}
	return 0;
}


