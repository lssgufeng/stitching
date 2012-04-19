#include "HarrisDetector.h"
HarrisDetector::HarrisDetector():neighbourhood(3),aperture(3),
	k(0.01),maxStrength(0.0),
	threshold(0.01),nonMaxSize(3){
}

void HarrisDetector::detect(const cv::Mat& image){

	cv::cornerHarris(image,cornerStrength,
		neighbourhood,
		aperture,
		k);
	//internal threshold computation
	double minStrength;
	cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);
	//local maxima detection
	cv::Mat dilated;
	cv::dilate(cornerStrength,dilated,cv::Mat());
	cv::compare(cornerStrength,dilated,localMax,cv::CMP_EQ);
}