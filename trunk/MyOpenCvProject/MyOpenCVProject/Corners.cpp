#include "Corners.h"
#include "Arithmatic.h"
#include "MyFilter.h"


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
		double threshold=50;
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
		double imageInformation=getImageInformation(image);
		printf("Image Information=%f",imageInformation);
		double threshold=pow(1.71,imageInformation/50)*20;
		printf("Threshold=%f",threshold);
		threshold=2;
		double minThreshold=1.0;
		double maxThreshold=1000.0;
		cv::Ptr<cv::AdjusterAdapter> adjuster=new cv::SurfAdjuster(threshold,minThreshold,maxThreshold);
		this->dymanicDetector=new cv::DynamicAdaptedFeatureDetector(adjuster,minFeatures,maxFeatures,iterations);
		this->dymanicDetector->detect(image,keyPoints);		
}

void Corners::GetDynamicAdaptedFeatures_FAST(cv::Mat& image,int minFeatures, 
		int maxFeatures,std::vector<cv::KeyPoint>& keyPoints,int iterations){
			double threshold=70.0;
			double minThreshold=1.0;
			double maxThreshold=1000.0;
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


double Corners::getImageInformation(cv::Mat& image ){
	/**For thresholding***/
	Arithmatic arithmatic;
	MyFilter filter;
	cv::Mat output;
	filter.Filter_Sobel(image,output);
	double sd1=arithmatic.CalculateSD(output);
	printf("standard deviation=%f",sd1);
	return sd1;
}




