// Experimental.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<stdio.h>
//#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"


int main(void)
{
	char* path1="C:/Users/krishna/Google Drive/images/Thesis/hc_16.png";
	char* path2="C:/Users/krishna/Google Drive/images/Thesis/rot_little_br_16.png";	
	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::imshow("Image1",image1);
	cv::waitKey(0);
}