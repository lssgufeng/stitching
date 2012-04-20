/**
 * @function main
 * @brief main function as entry point. 
 * @author Krishna
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "HarrisDetector.h"
#include "Corners.h";
#include <iostream>

int main(void)
{
	HarrisDetector detector;
	cv::Mat image=cv::imread("2.png",0);
	if(!image.data){
		printf("Error: Image Not Found!");
		std::getchar();		
	}
	detector.detect(image);
	std::vector<cv::Point> pts;
	detector.getCorners(pts,0.01);
	detector.drawOnImage(image,pts);
	cv::waitKey(0);

	Corners corner;
	std::vector<cv::KeyPoint> keyPoints;
	corner.GetGoodFeaturesToTrack(image,keyPoints);
	//corner.GetFastFeatures(image,keyPoints);
	//corner.GetSiftFeatures(image,keyPoints);
	//corner.GetSurfFeatures(image,keyPoints);
	printf("Key Points=%d",keyPoints.size());
	cv::Mat outputImage;
	//cv::SiftFeatureDetector sift(0.01,10);
	//sift.detect(image,keyPoints);

	corner.DrawKeyPoints(image,keyPoints,outputImage);
	cv::imshow("KeyPoints",outputImage);
	cv::waitKey(0);	
}
