// Experimental.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<stdio.h>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"

void ExtractHarrisFeatures();
void ExtractSIFTFeatures();
void ExtractSUFTFeatures();

int main(void)
{
	//Testing for feature extraction
	//ExtractHarrisFeatures();
	//ExtractSIFTFeatures()
	ExtractSUFTFeatures();
}

void ExtractHarrisFeatures(){	
	//HARRIS	
	char* path1="images/l_large_br_rot.jpg";
	char* savePath="result/harris/l_large_br_rot.jpg";
	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);	
	//Harris Detector
	int neighbourhood=3;
	int aperture=11;
	double k=0.04;
	double maxStrength=4;
	double threshold=0.000001;
	int nonMaxSize=12;


	cv::Mat cornerStrength;
	//harrisResult.create(image1.rows,image1.cols,CV_32FC1);
	cv::cornerHarris(image1,cornerStrength, neighbourhood,aperture,k,cv::BORDER_DEFAULT);
	//internal threshold computation
	double minStrength;
	cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);
	//local maxima detection
	cv::Mat dilated;
	cv::dilate(cornerStrength,dilated,cv::Mat());
	cv::Mat localMax;
	cv::compare(cornerStrength,dilated,localMax,cv::CMP_EQ);

	double qualityLevel=0.04;
	threshold= qualityLevel*maxStrength;
	cv::Mat cornerTh;
	cv::threshold(cornerStrength,cornerTh,
		threshold,255,cv::THRESH_BINARY);
	cv::Mat cornerMap;
	cornerTh.convertTo(cornerMap,CV_8U);
	//non-maxima suppression
	cv::bitwise_and(cornerMap,localMax,cornerMap);

	std::vector<cv::Point> points;
	for(int y=0;y<cornerMap.rows;y++){
			 const uchar* rowPtr=cornerMap.ptr<uchar>(y);
			 for(int x=0;x<cornerMap.cols;x++){
				 //if it is a feature point
				 if(rowPtr[x]){
					 points.push_back(cv::Point(x,y));
				 }
			 }
		 }

	std::vector<cv::Point>::const_iterator it=points.begin();
		 //for all corners
		 while(it!=points.end()){
			 //draw a circle
			 cv::circle(image1,*it,3, cv::Scalar(0),2);
			 ++it;
		 }		 

    cv::imwrite(savePath,image1);
	cv::imshow(savePath, image1);
	cv::waitKey(0);
}

void ExtractSIFTFeatures(){
	char* path1="images/l_rot_8.jpg";
	char* savePath="result/SIFT/l_rot_8.jpg";
	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	double threshold=0.04;
	double linesThreshold=10;
	std::vector<cv::KeyPoint> keyPoints;
	int64 tick=cv::getTickCount();
	cv::Ptr<cv::FeatureDetector> detector=new cv::SiftFeatureDetector(threshold,linesThreshold);
	detector->detect(image1,keyPoints);
	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();
	printf("ElapsedTime=%f",elapsedTime);
	cv::Mat tmpImage;
	cv::drawKeypoints(image1,keyPoints,tmpImage);
	cv::imwrite(savePath,tmpImage);
	cv::imshow(savePath,tmpImage);
	cv::waitKey(0);
}

void ExtractSUFTFeatures(){	
	char* path1="images/l_rot_8.jpg";
	char* savePath="result/SURF/l_rot_8.jpg";
	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	double threshold=700;
	std::vector<cv::KeyPoint> keyPoints;
	int64 tick=cv::getTickCount();
	cv::Ptr<cv::FeatureDetector> detector=new cv::SurfFeatureDetector(threshold);
	detector->detect(image1,keyPoints);
	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();


	cv::Mat tmpImage;
	cv::drawKeypoints(image1,keyPoints,tmpImage);
	cv::imwrite(savePath,tmpImage);
	cv::imshow(savePath,tmpImage);
	cv::waitKey(0);
}




