#include "Matching.h"

void Matching::GetMatchesBrief(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches){
		this->extractor=new cv::BriefDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches);
}

void Matching::GetMatchesSurf(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches){
		this->extractor=new cv::SurfDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches);
}

void Matching::GetMatchesSift(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches){
		this->extractor=new cv::SiftDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches);
}

void Matching::performMatching(cv::Mat descriptors1, cv::Mat descriptors2,
	std::vector<cv::DMatch>& matches){
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	matcher.match(descriptors1,descriptors2,matches);
}


