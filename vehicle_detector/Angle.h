/*
 * angle.h
 *
 *  Created on: Aug 30, 2016
 *      Author: Jolie Tran
 */

#ifndef ANGLE_H_
#define ANGLE_H_

using namespace cv;

class Angle {
private:
	int x1,x2,y1,y2;
	Mat frame;


public:
	void drawingLaneLine(Mat frame, int pointTop, int pointBottom) {
		pointTop(x1,y1);
		pointBottom(x2,y2);

		line(frame,Point pointTop,Point pointBottom,Scalar(255,255,0),3,CV_AA);


	}
};





#endif /* ANGLE_H_ */
