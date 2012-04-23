#include "Matching.h"

void Matching::GetMatchesBrief(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches1,std::vector<cv::DMatch>& matches2){
		this->extractor=new cv::BriefDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
}

void Matching::GetMatchesSurf(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches1,std::vector<cv::DMatch>& matches2){
		this->extractor=new cv::SurfDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
}

void Matching::GetMatchesSift(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches1,std::vector<cv::DMatch>& matches2){
		this->extractor=new cv::SiftDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
}

void Matching::performMatching(cv::Mat descriptors1, cv::Mat descriptors2,
	std::vector<cv::DMatch>& matches1,std::vector<cv::DMatch>& matches2){
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	matcher.match(descriptors1,descriptors2,matches1);
	matcher.match(descriptors2,descriptors1,matches2);
}

int Matching::RatioTest(std::vector<std::vector<cv::DMatch>>& matches,double threshold){
	int removed=0;
	for(std::vector<std::vector<cv::DMatch>>::iterator matchIterator=matches.begin();
		matchIterator!=matches.end();++matchIterator){
			if(matchIterator->size()>1){
				if((*matchIterator)[0].distance/(*matchIterator)[1].distance>threshold){
					matchIterator->clear();					
					removed++;
				}
			}else{
				matchIterator->clear();
				removed++;
			}
	}
	return removed;
}

void Matching::SymmetryTest(const std::vector<cv::DMatch>& matches1,
		const std::vector<cv::DMatch>& matches2,
		std::vector<cv::DMatch>& symMatches){
}

cv::Mat Matching::RansacTest(const std::vector<cv::DMatch>& matches,
		const std::vector<cv::KeyPoint>& keyPoints1,
		const std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<cv::DMatch>& resultMatches){
}





