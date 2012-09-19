// Experimental.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<stdio.h>
#include<time.h>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "MyLog.h"

std::vector<cv::Point> ExtractHarrisFeatures(char*,char*);
std::vector<cv::KeyPoint> ExtractSIFTFeatures(char*,char*);
std::vector<cv::KeyPoint> ExtractSUFTFeatures(char*,char*);



char files[][100]={"l.jpg","l_br.jpg","l_rot_8.jpg","l_large.jpg","l_br_rot.jpg","l_large_br.jpg","l_large_br_rot.jpg","l_noise.jpg"};

int main(void)
{
	//Testing for feature extraction
	for(int i=0; i<8; i++){
		ExtractHarrisFeatures(files[i],"result/harris/result.txt");
		ExtractSIFTFeatures(files[i],"result/SIFT/result.txt");
		ExtractSUFTFeatures(files[i],"result/SURF/result.txt");
	}
	
}

std::vector<cv::Point> ExtractHarrisFeatures(char* imageFile,char* resultFile){	
	MyLog log;
	time_t curr;
	time(&curr);	
	log.Write(resultFile,ctime(&curr));
	char* imageFullPath=new char[200];
	char* saveFullPath=new char[200];

	std::vector<cv::Point> basePoints;
	strcpy(imageFullPath,"images/");
	imageFullPath=strcat(imageFullPath,imageFile);
	strcpy(saveFullPath,"result/harris/");
	saveFullPath=strcat(saveFullPath,imageFile);

	cv::Mat image1=cv::imread(imageFullPath,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);	
	//Harris Detector
	int neighbourhood=3;
	int aperture=11;
	double k=0.04;
	double maxStrength=4;
	double threshold=0.000001;
	int nonMaxSize=12;

	//LOG
	log.Write(resultFile,"Image Supplied:%s",imageFullPath);


	cv::Mat cornerStrength;
	//harrisResult.create(image1.rows,image1.cols,CV_32FC1);
	int64 tick=cv::getTickCount();
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

	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();
	
	//LOG 
	log.Write(resultFile,"Elapsed Time=%f seconds",elapsedTime);
	log.Write(resultFile,"Got %d points",points.size());

	std::vector<cv::Point>::const_iterator it=points.begin();
		 //for all corners
		 while(it!=points.end()){
			 //draw a circle
			 cv::circle(image1,*it,3, cv::Scalar(0),2);
			 ++it;
		 }		 

		 //cv::imwrite(fullSavePath,image1);
		 return points;
}

std::vector<cv::KeyPoint> ExtractSIFTFeatures(char* imageFile, char* resultFile){
	MyLog log;
	time_t curr;
	time(&curr);	
	log.Write(resultFile,ctime(&curr));
	char* imageFullPath=new char[200];
	char* saveFullPath=new char[200];

	std::vector<cv::Point> basePoints;
	strcpy(imageFullPath,"images/");
	imageFullPath=strcat(imageFullPath,imageFile);
	strcpy(saveFullPath,"result/SIFT/");
	saveFullPath=strcat(saveFullPath,imageFile);
	cv::Mat image1=cv::imread(imageFullPath,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	//LOG
	log.Write(resultFile,"Image Supplied:%s",imageFullPath);

	double threshold=0.04;
	double linesThreshold=10;
	std::vector<cv::KeyPoint> keyPoints;
	int64 tick=cv::getTickCount();
	cv::Ptr<cv::FeatureDetector> detector=new cv::SiftFeatureDetector(threshold,linesThreshold);
	detector->detect(image1,keyPoints);
	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();

	log.Write(resultFile,"Elapsed Time=%f Seconds.",elapsedTime);
	log.Write(resultFile,"Got %d key points",keyPoints.size());
	cv::Mat tmpImage;
	cv::drawKeypoints(image1,keyPoints,tmpImage);
	cv::imwrite(saveFullPath,tmpImage);	
	return keyPoints;
}

std::vector<cv::KeyPoint> ExtractSUFTFeatures(char* imageFile, char* resultFile){	
	MyLog log;
	time_t curr;
	time(&curr);	
	log.Write(resultFile,ctime(&curr));
	char* imageFullPath=new char[200];
	char* saveFullPath=new char[200];

	std::vector<cv::Point> basePoints;
	strcpy(imageFullPath,"images/");
	imageFullPath=strcat(imageFullPath,imageFile);
	strcpy(saveFullPath,"result/SURF/");
	saveFullPath=strcat(saveFullPath,imageFile);
	cv::Mat image1=cv::imread(imageFullPath,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	//LOG
	log.Write(resultFile,"Image Supplied:%s",imageFullPath);

	double threshold=700;
	std::vector<cv::KeyPoint> keyPoints;
	int64 tick=cv::getTickCount();
	cv::Ptr<cv::FeatureDetector> detector=new cv::SurfFeatureDetector(threshold);
	detector->detect(image1,keyPoints);
	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();

	log.Write(resultFile,"Elapsed Time=%f Seconds.",elapsedTime);
	log.Write(resultFile,"Got %d key points",keyPoints.size());

	cv::Mat tmpImage;
	cv::drawKeypoints(image1,keyPoints,tmpImage);
	cv::imwrite(saveFullPath,tmpImage);
	return keyPoints;
}


