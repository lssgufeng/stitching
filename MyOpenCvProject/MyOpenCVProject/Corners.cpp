#include "Corners.h"

Corners::Corners(){
}

void Corners::GetGoodFeaturesToTrack(
	const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints){
		int maxCorners=500;
		double qualityLevel=0.001;
		int minDistance=15;
		cv::GoodFeaturesToTrackDetector gftt(maxCorners,
			qualityLevel,minDistance);
		gftt.detect(image,keyPoints);
}

void Corners::GetFastFeatures(
	const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints){
		int threshold=5;
		cv::FastFeatureDetector fast(threshold);	
		fast.detect(image,keyPoints);		
}

void Corners::GetSurfFeatures(const cv::Mat& image,
	std::vector<cv::KeyPoint>& keyPoints){
		double threshold=20;
		cv::SurfFeatureDetector surf(threshold);
		surf.detect(image,keyPoints);
}
void Corners::GetSiftFeatures(const cv::Mat& image,
	std::vector<cv::KeyPoint>& keyPoints){
		double threshold=0.01;
		double linesThreshold=10;
		cv::SiftFeatureDetector sift(threshold,linesThreshold);
		sift.detect(image,keyPoints);
}


void Corners::DrawKeyPoints(const cv::Mat& originalImage,
	std::vector<cv::KeyPoint>& keyPoints,cv::Mat& outputImage){
		cv::drawKeypoints(originalImage,keyPoints,outputImage,
			cv::Scalar(-1));
}



