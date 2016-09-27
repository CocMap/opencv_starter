#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

//include time library
#include <stdio.h>
#include <time.h>

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	int frameCount;
	Mat frame, preFrame, grayDiff, frameDiff;
	Mat fgMaskMOG2;
	Mat grayFrame, blurFrame, Canny_output;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Ptr<BackgroundSubtractorMOG2> pMOG;

	//declare variable for motion
	double vehicle_speed_mps = 0, vehicle_distance = 0, vehicle_time = 0;
	double vehicle_speed_kmph;

	//declare variables for Canny
	int const lowThres = 100;
	int ratio = 3;
	int highThres = ratio*lowThres;
	int kernel_size = 3;

	char videoFileName[50] = ("Computing.MOV");
	VideoCapture cap("Computing.MOV");
	if (!cap.isOpened())
	{
		cout << "Error loading camera, please check your camera connection! "
			<< endl;											//check if the camera open successfully
		return -1;
	}

	pMOG = createBackgroundSubtractorMOG2();
	pMOG->setVarThreshold(100);

	cap >> frame;
	int cols = frame.cols;
	int rows = frame.rows;

	cout << "Frame cols are: " << cols << endl;
	cout << "Frame rows are: " << rows << endl << endl;

	//create window
	namedWindow("Original", CV_WINDOW_NORMAL);
	namedWindow("Process", CV_WINDOW_NORMAL);

	int currentFrame = 0;

	while (true)
	{
		int t1 = 0;
		int t2 = 0;

		//flag to check if t1 and t2 is modified for each vehicle
		bool t1Modified = false;
		bool t2Modified = false;
		
		//get lowest position of vehicle to decide which lane vehicle is moving on
		int maxRow = 0;

		//flag to check if calculations is done
		bool isFinished = false;

		//frame which calculations is done
		int finishedFrame;

		//store maximum height and minimum y of tracking box size
		int iHeight = -10;
		int iY = rows + 10;

		for (int frameCount = currentFrame; ; frameCount++)
		{
			frame.copyTo(preFrame);
			cap >> frame;

			//temp frame to draw tracking box
			Mat temp;
			frame.copyTo(temp);

			pMOG->apply(frame, fgMaskMOG2);

			morphologyEx(fgMaskMOG2, fgMaskMOG2, MORPH_OPEN, kernel);
			vector < vector < Point > >contours;


			absdiff(frame, preFrame, frameDiff);
			cvtColor(frameDiff, grayDiff, CV_RGB2GRAY);

			GaussianBlur(grayDiff, blurFrame, Size(3, 3), 7, 7);	 		//filter the noise using GaussianBlur
			Canny(blurFrame, Canny_output, lowThres, highThres, kernel_size);	//Canny edges

			double fps = cap.get(CV_CAP_PROP_FPS);		//fps = 59.9535ms

			for (int y = rows - 1; y >= 0; y--)
			{
				if (Canny_output.at<uchar>(y, 5) > 250)
				{
					t1 = frameCount;

					if (y > maxRow)
					{
						maxRow = y;
					}

					if (t1 != 0 || !t1Modified)
					{
						t1Modified = true;
					}
					break;
				}
			}


			for (int y = rows - 1; y >= 0; y--)
			{
				if (Canny_output.at<uchar>(y, cols - 5) > 250)
				{
					t2 = frameCount;

					if (y > maxRow)
					{
						maxRow = y;
					}

					if (t2 != 0 || !t2Modified)
					{
						t2Modified = true;
					}
					break;
				}
			}

			//draw tracking box
			findContours(fgMaskMOG2.clone(), contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

			if (!contours.empty())
			{
				// Get largest contour
				int idx_largest_contour = -1;
				double area_largest_contour = 0.0;

				for (size_t i = 0; i < contours.size(); ++i)
				{
					double area = contourArea(contours[i]);
					if (area_largest_contour < area)
					{
						area_largest_contour = area;
						idx_largest_contour = i;
					}
				}

				if (area_largest_contour > 200)
				{
					// Draw
					Rect roi = boundingRect(contours[idx_largest_contour]);

					if (roi.height > rows - 30)
					{
						if (iHeight < roi.height || iHeight <= 0)
						{
							iHeight = roi.height;
						}
						else
						{

							roi.height = iHeight;
						}
					}

					if (roi.y < 30)
					{
						if (iY > roi.y || iY >= rows)
						{
							iY = roi.y;
						}
						else
						{
							roi.y = iY;
						}
					}

					rectangle(temp, roi, Scalar(0, 255, 255));
				}
			}
			
			if (t1 != 0 && t2 != 0) 
			{
				if (!isFinished)
				{
					finishedFrame = frameCount;
					isFinished = true;
				}
				else
				{
					//delay calculations by 35 frame to make sure t1 and t2 unchanged due to OpenCV's multithread
					if (frameCount > (finishedFrame + 35))
					{
						if (maxRow <= 400)
						{
							vehicle_distance = 16.7;
						}
						else if (maxRow <= 430)
						{
							vehicle_distance = 13.7;
						}
						else if (maxRow <= 500)
						{
							vehicle_distance = 10.7;
						}
						else
						{
							vehicle_distance = 7.9;
						}


						vehicle_time = abs((t2 - t1)*(1 / fps));	//s
						vehicle_speed_mps = vehicle_distance / vehicle_time;

						cout << "Distance: " << vehicle_distance << " m" << endl;
						cout << "Time: " << vehicle_time << " s" << endl;
						cout << "Speed in m/s: " << vehicle_speed_mps << " m/s" << endl;
						vehicle_speed_kmph = vehicle_speed_mps * 3600 / 1000;
						cout << "Speed in km/h: " << vehicle_speed_kmph << " km/h" << endl;

						//reset t1 and t2
						t1 = 0;
						t2 = 0;

						currentFrame = frameCount;
						isFinished = false;

						//reset tracking box height and y
						iHeight = -10;
						iY = rows + 10;
						break;
					}
				}
			}


			//print results on "Process" window
			char Buffer[100];
			sprintf(Buffer, "Distance: %4.2f", vehicle_distance);
			putText(Canny_output, Buffer, Point(20, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, 8);

			sprintf(Buffer, "Time: %4.2f", vehicle_time);
			putText(Canny_output, Buffer, Point(20, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, 8);

			sprintf(Buffer, "Speed in m/s: %4.2f", vehicle_speed_mps);
			putText(Canny_output, Buffer, Point(20, 150), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, 8);

			sprintf(Buffer, "Speed in km/h: %4.2f", vehicle_speed_kmph);
			putText(Canny_output, Buffer, Point(20, 200), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, 8);

			imshow("Original", temp);
			imshow("Process", Canny_output);

			if (waitKey(20) >= 0)
			{
				return 0;
			}
		}
	}
}
