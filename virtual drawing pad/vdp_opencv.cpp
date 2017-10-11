/*
*********************************************************							
*							*
*    Aim : Virtual Drawing Pad                          *
*    Author : Aniket : IvLabs 				*
*							*
*********************************************************
*/

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
int main(int argc, char* argv[])
{
	VideoCapture cap(1);
	if ( !cap.isOpened() ) // if not success, exit program
	{
	cout << "Cannot open the video file" << endl;
	return -1;
	}
// open the video file for reading
	namedWindow("src",1);
int iLastX = -1; 
int iLastY = -1;
Mat temp;
 cap.read(temp); 
 
 Mat imgLines = Mat::zeros(temp.size(), CV_8UC3 );
while(1)
{
	Mat src;
	bool bSuccess = cap.read(src); // read a new frame from video
	if (!bSuccess) //if not success, break loop
	break;
        
       flip(src, src, 1); 
       
// Split the image into different channels
	vector<Mat> rgbChannels(3);
	split(src, rgbChannels);
	Mat g,new_rgb,diff_red,diff_blue,diff_green,gray;
	g = Mat::ones(Size(src.cols, src.rows), CV_8UC1);
	diff_red=(rgbChannels[2]-rgbChannels[1]);
	diff_green=(rgbChannels[1]-rgbChannels[0]);
	diff_blue=(rgbChannels[0]-rgbChannels[2]);
	vector<Mat> channels;
	channels.push_back(diff_blue);
	channels.push_back(diff_green);
	channels.push_back(diff_red);
	merge(channels, new_rgb);
	Mat extract_yellow,thresholdedr,extract_red,extract_blue,extract_green,thresholdedg,thresholdedy,temp,diff_yellow,thresholdedb;
	cvtColor(new_rgb,gray,COLOR_BGR2GRAY);
	extract_blue=(diff_blue) - gray ;
	GaussianBlur(extract_blue,extract_blue,Size(5,5),0) ;
	threshold(extract_blue,thresholdedb,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU);

	erode(thresholdedb,thresholdedb, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate(thresholdedb, thresholdedb, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//morphological closing (fill small holes in the foreground)
	dilate(thresholdedb,thresholdedb, getStructuringElement(MORPH_ELLIPSE,Size(5, 5)) );
	erode(thresholdedb, thresholdedb, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        Moments oMoments = moments(thresholdedb);

  	double dM01 = oMoments.m01;
  	double dM10 = oMoments.m10;
  	double dArea = oMoments.m00;

// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
  	if (dArea > 10000)
  	{
//calculate the position of the ball
  	int posX = dM10 / dArea;
   	int posY = dM01 / dArea;        
        
   	if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
   	{
    //Draw a red line from the previous point to the current point
   	 line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
   	}

   iLastX = posX;
   iLastY = posY;
  }

 
 src = src + imgLines;
  imshow("src", src); //show the original image



	namedWindow("thresholded",1);imshow("thresholded", thresholdedb);
	imshow("src",src);
	if(waitKey(3) == 27)
	break;
}
return 0;
}
