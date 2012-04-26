#include "Corners.h"

Corners::Corners(){
}

void Corners::GetGoodFeaturesToTrack(
	const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints){
		int maxCorners=500;
		double qualityLevel=0.001;
		int minDistance=15;
		this->detector= new cv::GoodFeaturesToTrackDetector(maxCorners,
			qualityLevel,minDistance);
		this->detector->detect(image,keyPoints);
}

void Corners::GetFastFeatures(
	const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints){
		int threshold=70;
		this->detector= new cv::FastFeatureDetector(threshold);	
		this->detector->detect(image,keyPoints);		
}

void Corners::GetSurfFeatures(const cv::Mat& image,
	std::vector<cv::KeyPoint>& keyPoints){
		double threshold=5;
		this->detector=new cv::SurfFeatureDetector(threshold);
		this->detector->detect(image,keyPoints);
}
void Corners::GetSiftFeatures(const cv::Mat& image,
	std::vector<cv::KeyPoint>& keyPoints){
		double threshold=0.01;
		double linesThreshold=10;
		this->detector=new cv::SiftFeatureDetector(threshold,linesThreshold);
		this->detector->detect(image,keyPoints);
}
void Corners::GetDynamicAdaptedFeatures_SURF(cv::Mat& image,int minFeatures,
	int maxFeatures,std::vector<cv::KeyPoint>& keyPoints,
	int iterations){
		double threshold=5;
		double minThreshold=1.0;
		double maxThreshold=1000;
		cv::Ptr<cv::AdjusterAdapter> adjuster=new cv::SurfAdjuster(threshold,minThreshold,maxThreshold);
		this->dymanicDetector=new cv::DynamicAdaptedFeatureDetector(adjuster,minFeatures,maxFeatures,iterations);
		this->dymanicDetector->detect(image,keyPoints);
}

void Corners::GetDynamicAdaptedFeatures_FAST(cv::Mat& image,int minFeatures, 
		int maxFeatures,std::vector<cv::KeyPoint>& keyPoints,int iterations){
			double threshold=70;
			double minThreshold=1.0;
			double maxThreshold=1000;
			cv::Ptr<cv::AdjusterAdapter> adjuster=new cv::FastAdjuster(threshold,true,minThreshold,maxThreshold);
			this->dymanicDetector=new cv::DynamicAdaptedFeatureDetector(adjuster,minFeatures,maxFeatures,iterations);
			this->dymanicDetector->detect(image,keyPoints);
}


//To obtain the corner points within a defined range
	void Corners::GetDynamicAdaptedFeatures_STAR(cv::Mat& image,int minFeatures, 
		int maxFeatures,std::vector<cv::KeyPoint>& keyPoints,int iterations){
			double threshold=70;
			double minThreshold=1.0;
			double maxThreshold=1000;
			cv::Ptr<cv::AdjusterAdapter> adjuster=new cv::StarAdjuster(threshold,minThreshold,maxThreshold);
			this->dymanicDetector=new cv::DynamicAdaptedFeatureDetector(adjuster,minFeatures,maxFeatures,iterations);
			this->dymanicDetector->detect(image,keyPoints);
	}


void Corners::DrawKeyPoints(const cv::Mat& originalImage,
	std::vector<cv::KeyPoint>& keyPoints,cv::Mat& outputImage){
		cv::drawKeypoints(originalImage,keyPoints,outputImage,
			cv::Scalar(-1));
}





