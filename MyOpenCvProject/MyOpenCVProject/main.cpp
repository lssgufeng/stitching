/**
 * @function cornerHarris_Demo.cpp
 * @brief Demo code for detecting corners using Harris-Stephens method
 * @author OpenCV team
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "HarrisDetector.h"
#include <iostream>

int main(void)
{
	HarrisDetector detector;
	cv::Mat image=cv::imread("photo.jpg",0);
	if(!image.data){
		std::cout<<"Error: Image Not Found!";
		std::getchar();
	}
	detector.detect(image);

}
