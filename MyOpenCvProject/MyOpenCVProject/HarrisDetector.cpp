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
}

cv::Mat HarrisDetector::getCornerMap(double qualityLevel){
	cv::Mat cornerMap;
	//thresholding the corner strength
	threshold= qualityLevel*maxStrength;
	cv::threshold(cornerStrength,cornerTh,
		threshold,255,cv::THRESH_BINARY);
	cornerTh.convertTo(cornerMap,CV_8U);
	//non-maxima suppression
	cv::bitwise_and(cornerMap,localMax,cornerMap);
	return cornerMap;
}

 void HarrisDetector::getCorners(std::vector<cv::Point>& points, 
	 const cv::Mat& cornerMap){
		 for(int y=0;y<cornerMap.rows;y++){
			 const uchar* rowPtr=cornerMap.ptr<uchar>(y);
			 for(int x=0;x<cornerMap.cols;x++){
				 //if it is a feature point
				 if(rowPtr[x]){
					 points.push_back(cv::Point(x,y));
				 }
			 }
		 }
 }

 void HarrisDetector::getCorners(std::vector<cv::Point>& points,
		double qualityLevel){
		//Get the corner map
			cv::Mat cornerMap=getCornerMap(qualityLevel);
			//get the corners
			getCorners(points,cornerMap);
 }

 void HarrisDetector::drawOnImage(cv::Mat& image, const std::vector<cv::Point>& points,
	 cv::Scalar color, int radius,int thickness){
		 std::vector<cv::Point>::const_iterator it=points.begin();
		 //for all corners
		 while(it!=points.end()){
			 //draw a circle
			 cv::circle(image,*it,radius, color,thickness);
			 ++it;
		 }

 }



