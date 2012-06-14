//#include "Stitching.h"
#include "Warp.h"
#include "AlphaBlender.h"
#include "Corners.h"
#include "Matching.h"
#include "Utility.h"


Stitching::Stitching(cv::Mat floatingImage,
	cv::Mat baseImage){
		this->floatingImage=floatingImage;
		this->baseImage=baseImage;		
}

Stitching::~Stitching(){
}
void Stitching::Stitch(){	
	cv::Mat homography=calculateHomography(this->floatingImage,this->baseImage);
	Warp warp;

	cv::Point topLeft, bottomRight;
	warp.RotateImage(this->floatingImage,homography,this->rotatedImage,topLeft,bottomRight);	
	cv::imwrite("output/rotatedImage.png",this->rotatedImage);
}







	




cv::Mat Stitching::calculateHomography(cv::Mat image1,cv::Mat image2){
	Corners corner;
	std::vector<cv::KeyPoint> keyPoints1,keyPoints2;	
	corner.GetSurfFeatures(image1,keyPoints1);
	corner.GetSurfFeatures(image2,keyPoints2);
	Matching matching;
	std::vector<std::vector<cv::DMatch>> matches1,matches2;
	matching.GetMatchesSurf(image1,image2,keyPoints1,keyPoints2,matches1,matches2);	
	int removed1=matching.RatioTest(matches1,0.8);	
	int removed2=matching.RatioTest(matches2,0.8);	
	std::vector<cv::DMatch> symmetryMatches;
	matching.SymmetryTest(matches1,matches2,symmetryMatches);
	cv::Mat imageMatches;	
	std::vector<uchar> inliers;
	cv::Mat homography;
	homography=matching.GetHomography(symmetryMatches,keyPoints1,keyPoints2,inliers);	
	//homography.at<double>(2,0)=homography.at<double>(2,1)=0;
	Utility utility;
	utility.WriteHomography("Homography",homography);
	return homography;
}





