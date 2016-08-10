#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
      Mat image = imread ("testingImage.png");


      if(!image.data) {
                  return -1;
            }

      namedWindow("Original", CV_WINDOW_AUTOSIZE);                      //create new window
      namedWindow("Gray Image", CV_WINDOW_AUTOSIZE);
      namedWindow("Result", CV_WINDOW_AUTOSIZE);

      Mat grayImage;
      cvtColor(image, grayImage, CV_BGR2GRAY);

      int x = grayImage.cols;
      int y = grayImage.rows;

      int i, j;
      for (i = 0; i < x; i++) {
            for (j = 0; j < y; j++) {
                  if (grayImage.at<uchar> (j,i) == 255) {
                        grayImage.at<uchar> (j,i) = 0;
                  }
            }
      }
      imshow("Original", image);

      imshow("Gray Image", grayImage);




      waitKey(0);
      return(0);
}
