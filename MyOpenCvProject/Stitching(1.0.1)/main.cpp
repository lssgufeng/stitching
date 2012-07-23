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
#include "Corners.h"
#include "Matching.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string.h>
#include "MyTimer.h"
#include "Arithmatic.h"
#include "MyFilter.h"
#include "Warp.h"
#include "Utility.h"
#include "Stitching.h"

#define DllExport __declspec(dllexport)
extern "C" DllExport void  Stitch(LPSTR path1, LPSTR path2,int direction,int method);

extern "C" DllExport void StitchRaw(unsigned short* image1, unsigned short* image2);

/* Method that stitches two images*/
cv::Mat Stitch(cv::Mat image1, cv::Mat image2,int direction);
cv::Mat Stitch_Flann(cv::Mat image1, cv::Mat image2,int direction);

int main(void)
{
	//NOT Required
	char* path1="C:/Users/krpaudel/Google Drive/images/16bit/13_16.png";
	char* path2="C:/Users/krpaudel/Google Drive/images/16bit/3_16.png";
	
	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	int64 tick=cv::getTickCount();
	cv::Mat stitchedImage=Stitch_Flann(image1, image2,2);

	cv::imwrite("output/stitchedImage.png",stitchedImage);
	float seconds=(cv::getTickCount()-tick)/cv::getTickFrequency();

	printf("Stiching Took %f seconds",seconds);

	getchar();

}
cv::Mat Stitch(cv::Mat image1, cv::Mat image2,int direction){
	for(int i=0;i<image1.rows;i++){
		for(int j=0;j<image1.cols;j++){
			if(image1.at<ushort>(i,j)==0){
				image1.at<ushort>(i,j)=1;
			}			
		}
	}
	for(int i=0;i<image2.rows;i++){
		for(int j=0;j<image2.cols;j++){
			if(image2.at<ushort>(i,j)==0){
				image2.at<ushort>(i,j)=1;
			}			
		}
	}
	Stitching stitching(image1,image2);
	return stitching.Stitch(direction);	
}

cv::Mat Stitch_Flann(cv::Mat image1, cv::Mat image2, int direction){
	for(int i=0;i<image1.rows;i++){
		for(int j=0;j<image1.cols;j++){
			if(image1.at<ushort>(i,j)==0){
				image1.at<ushort>(i,j)=1;
			}			
		}
	}
	for(int i=0;i<image2.rows;i++){
		for(int j=0;j<image2.cols;j++){
			if(image2.at<ushort>(i,j)==0){
				image2.at<ushort>(i,j)=1;
			}			
		}
	}
	Stitching stitching(image1,image2);
	return stitching.Stitch_Flann(direction);	
}


void Stitch(LPSTR path1, LPSTR path2,int direction, int method){
	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);	
	cv::imwrite("output/image1.png",image1);
	cv::imwrite("output/image2.png",image2);	
	if(method==0){
		Stitch(image1,image2,direction);
	}else{
		Stitch_Flann(image1,image2,direction);
	}
}


//unsigned short* StitchRaw(unsigned short* image1Data,int width1,int height1, 
//	unsigned short* image2Data,int width2,int height2){
//		cv::Mat image1(height1,width1,CV_16U,image1Data);
//		cv::Mat image2(height2,width2,CV_16U,image2Data);
//		cv::imwrite("output/image1_raw.png",image1);
//		cv::imwrite("output/image2_raw.png",image2);
//		cv::Mat stitched;
//		stitched=Stitch(image1,image2);
//		return image1Data;
//}