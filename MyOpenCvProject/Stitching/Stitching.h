/***
* @class Stitching
* @description This performs the final operations 
	of stitching. After we get the rotated image of 
	the floating image, the image is stitched with 
	the base image.
	Initial assumption is we have the rotated float image,the base image and homography
	###########
	TODO: implement everything here i.e. it takes two images and gives the final result. 
	###########
* @author Krishna Paudel(krishna444@gmail.com)
* @date 2012-05-08 13:58:34
***/

#pragma once
#include "opencv2\core\core.hpp"
#include "LaplacianBlender.h"
#include "MyLog.h"

class Stitching{
private:
	char szBuffer[100];
	MyLog* log;
	//Floating image
	cv::Mat floatingImage;
	//Base Image
	cv::Mat baseImage;
	//Rotated Image
	cv::Mat rotatedImage;

	//Homography
	cv::Mat homography;	


private:
	//Calculates the homography of image1 to the image2
	bool calculateHomography(cv::Mat image1,cv::Mat image2,cv::Mat& homography);

	bool calculateHomography_Flann(cv::Mat image1, cv::Mat image2, cv::Mat& homography);

	bool calculateHomography_Freak(cv::Mat image1, cv::Mat image2, cv::Mat& homography);

    //Calculates the common(overlap) area
	void calculateOverlapImages(const cv::Mat homography, 
		const cv::Mat floatImage,const cv::Mat baseImage,
		cv::Mat& outputFloatImage,cv::Mat& outputBaseImage);

public: 
	

	//Constructor. The size of the images should be same.
	//@floatingImage Floating Image
	//@baseImage Base Image, which is fixed.	
	Stitching(cv::Mat floatingImage, cv::Mat baseImage);
	//destructor
	~Stitching();
	//it performs the main task of stitching using exhaustive knn method
	//@direction Direction of stitching: vertical or horizontal
	//@crop Cropping allowed or not
	cv::Mat Stitch(int direction,bool crop);
	//it performs the main task of stitching using exhaustive ann(approximate nearest neighborhood  method)
	//@direction Direction of stitching: vertical or horizontal
	//@crop Cropping allowed or not
	cv::Mat Stitching::Stitch_Flann(int direction,bool crop);
	//void stich(cv::Mat base, cv::Mat target,cv::Mat homography, cv::Mat& panorama);
	cv::Mat Stitching::Stitch_Freak(int direction,bool crop);
};

