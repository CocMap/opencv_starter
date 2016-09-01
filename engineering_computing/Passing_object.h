/*
 * passing_object.h
 *
 *  Created on: Aug 30, 2016
 *      Author: Jolie Tran
 */

#ifndef PASSING_OBJECT_H_
#define PASSING_OBJECT_H_

#include <iostream>

using namespace cv;

class PassingObject {
private:
	Mat photo;
	int frameNumber;

public:
	void setPhoto(Mat inputPhoto){
		photo = inputPhoto;
	}

	Mat getPhoto() {
		return photo;
	}

	void setFrameNumber(int fn) {
		frameNumber = fn;
	}

	void printFrameNumber() {
		printf("When the object passed by, ");
		printf("The frame number was: %d.\n", frameNumber);
	}

	void savePhotoToFile(int count) {
		char fileName[100];		//an array of characters, or string
		sprintf(fileName, "passing_object_%03d-%06d.jpg", count, frameNumber);
		imwrite(fileName, photo);
	}
};


#endif /* PASSING_OBJECT_H_ */
