#include "Matching.h"

void Matching::GetMatchesBrief(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
		this->extractor=new cv::BriefDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
}

void Matching::GetMatchesSurf(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
		this->extractor=new cv::SurfDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
}

void Matching::GetMatchesSift(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
		this->extractor=new cv::SiftDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
}

void Matching::performMatching(cv::Mat descriptors1, cv::Mat descriptors2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
	cv::BruteForceMatcher<cv::L2<float>> matcher;
	matcher.knnMatch(descriptors1,descriptors2,matches1,2);
	matcher.knnMatch(descriptors2,descriptors1,matches2,2);
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

void Matching::SymmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
	const std::vector<std::vector<cv::DMatch>>& matches2,
		std::vector<cv::DMatch>& symMatches){
			//check for image1->imag2 matches
			for(std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator1=matches1.begin();
				matchIterator1!=matches1.end();++matchIterator1){
					if(matchIterator1->size()<2) continue;
					//check for image2->image1 matches
					for(std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator2=matches2.begin();
						matchIterator2!=matches2.end();++matchIterator2){
							if(matchIterator2->size()<2)continue;
							//Match Symmetry Test
							bool condition=(*matchIterator1)[0].queryIdx==(*matchIterator2)[0].trainIdx
								&&(*matchIterator1)[0].trainIdx==(*matchIterator2)[0].queryIdx;
							if(condition){
								symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx,
									(*matchIterator1)[0].trainIdx,(*matchIterator1)[0].distance));								
								break;
							}
					}
			}
}

cv::Mat Matching::RansacTest(const std::vector<cv::DMatch>& goodMatches,
		const std::vector<cv::KeyPoint>& keyPoints1,
		const std::vector<cv::KeyPoint>& keyPoints2,
		double distance, double confidence,
		std::vector<cv::DMatch>& resultMatches){
			//Convert keypoints into Point2f
			std::vector<cv::Point2f> points1,points2;
			this->GetFloatPoints(keyPoints1,keyPoints2,goodMatches,points1,points2);
			//Compute Fundamental Matrix
			std::vector<uchar> inliers(points1.size(),0);
			cv::Mat fundamental=cv::findFundamentalMat(cv::Mat(points1),
				cv::Mat(points2),
				inliers,
				CV_FM_RANSAC, //RANSAC Method
				distance, //distance to epipolar line
				confidence); //confidence probability
			
			//Get the inlier points
			std::vector<uchar>::const_iterator iteratorInlier=
				inliers.begin();
			std::vector<cv::DMatch>::const_iterator iteratorMatch=
				goodMatches.begin();
			for(;iteratorInlier!=inliers.end();++iteratorInlier,++iteratorMatch){
				if(*iteratorInlier){
					resultMatches.push_back(*iteratorMatch);
				}
			}

			//Refinement of Fundamental Matrix from the accepted matches
			points1.clear();
			points2.clear();
			this->GetFloatPoints(keyPoints1,keyPoints2,resultMatches,points1,points2);


			return fundamental;
}

cv::Mat Matching::GetHomography(const std::vector<cv::DMatch>& goodMatches,
	std::vector<cv::KeyPoint>& keyPoints1,
	std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<uchar>& inliers){
		std::vector<cv::Point2f> points1,points2;
		//convert to floating point for homography		
		this->GetFloatPoints(keyPoints1,keyPoints2,goodMatches,points1,points2);
		cv::Mat homography= cv::findHomography(
			cv::Mat(points1), //Corresponding 
			cv::Mat(points2), //matching points
			inliers,          //inliers
			CV_RANSAC,        //using RANSAC method
			1);        		  //maximum pixel distance		
		return homography;
}

void Matching::DrawMatches(cv::Mat image1,std::vector<cv::KeyPoint> keyPoints1,
		cv::Mat image2,std::vector<cv::KeyPoint> keyPoints2,
		std::vector<cv::DMatch> matches, cv::Mat& outputImage){
			cv::drawMatches(image1,keyPoints1,image2,keyPoints2,matches,outputImage,cv::Scalar(255,255,255));
}

void Matching::DrawInliers(std::vector<cv::Point2f> points,std::vector<uchar>& inliers, cv::Mat image,cv::Mat& outImage){
	outImage=image.clone();
	std::vector<cv::Point2f>::const_iterator iteratorMatches=points.begin();
	std::vector<uchar>::const_iterator iteratorInliers=inliers.begin();
	while(iteratorInliers!=inliers.end()){
		if(*iteratorInliers){
			cv::circle(outImage,*iteratorMatches,3,cv::Scalar(0,0,0));
		}
		++iteratorInliers;++iteratorMatches;
	}
}


void Matching::GetFloatPoints(const std::vector<cv::KeyPoint>& keyPoints1,const std::vector<cv::KeyPoint>& keyPoints2,
	const std::vector<cv::DMatch>& matches, 
	std::vector<cv::Point2f>& points1,
	std::vector<cv::Point2f>& points2){
		for(std::vector<cv::DMatch>::const_iterator iterator=matches.begin();
			iterator!=matches.end();
			++iterator){
				float x=keyPoints1[iterator->queryIdx].pt.x;
				float y=keyPoints1[iterator->queryIdx].pt.y;
				points1.push_back(cv::Point2f(x,y));
				x=keyPoints2[iterator->trainIdx].pt.x;
				y=keyPoints2[iterator->trainIdx].pt.y;
				points2.push_back(cv::Point2f(x,y));
	}
}








