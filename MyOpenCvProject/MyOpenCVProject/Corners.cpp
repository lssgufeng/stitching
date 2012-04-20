#include "Corners.h"

Corners::Corners():maxCorners(200),
	qualityLevel(0.1),minDistance(10),threshold(40){
}

void Corners::GetGoodFeaturesToTrack(
	const cv::Mat& image,std::vector<cv::KeyPoint> keyPoints){
		cv::GoodFeaturesToTrackDetector gftt(this->maxCorners,
			this->qualityLevel,this->minDistance);
		gftt.detect(image,keyPoints);
}

void Corners::GetFastFeatures(
	const cv::Mat& image,std::vector<cv::KeyPoint> keyPoints){
		cv::FastFeatureDetector fast(this->threshold);	
		fast.detect(image,keyPoints);
}

void Corners::DrawKeyPoints(const cv::Mat& originalImage,
	std::vector<cv::KeyPoint> keyPoints,cv::Mat& outputImage){
		cv::drawKeypoints(originalImage,keyPoints,outputImage,
			cv::Scalar(255,255,255));
}

