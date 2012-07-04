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
extern "C" DllExport void  Stitch(LPSTR path1, LPSTR path2);

/* Method that stitches two images*/
cv::Mat Stitch(cv::Mat image1, cv::Mat image2);
int main(void)
{
	//NOT Required
}
cv::Mat Stitch(cv::Mat image1, cv::Mat image2){
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
	return stitching.Stitch();	
}

void Stitch(LPSTR path1, LPSTR path2){
	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	Stitch(image1,image2);
}