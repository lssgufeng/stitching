#include "HarrisDetector.h"

HarrisDetector::HarrisDetector():neighbourhood(3),aperture(3),
	k(0.01),maxStrength(10),
	threshold(0.001),nonMaxSize(3){
}

void HarrisDetector::detect(const cv::Mat& image){
	//cornerStrength=cv::Mat::zeros(image.size(), CV_32FC1);
	cv::cornerHarris(image,cornerStrength,
		neighbourhood,aperture,
		k,cv::BORDER_DEFAULT);
	//internal threshold computation
	double minStrength;
	cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);
	//local maxima detection
	cv::Mat dilated;
	cv::dilate(cornerStrength,dilated,cv::Mat());
	cv::compare(cornerStrength,dilated,localMax,cv::CMP_EQ);
	//cv::imshow("dilated image",image);
	cv::waitKey(0);
}