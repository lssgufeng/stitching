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
		MyLog* log=new MyLog("Stitching.log");
		int i=2;
		log->Write("test%d",i);
}

Stitching::~Stitching(){
}

void Stitching::Stitch(){	
	cv::Mat homography=calculateHomography(this->floatingImage,this->baseImage);
	Warp warp;

	cv::Point topLeft, bottomRight;
	warp.RotateImage(this->floatingImage,homography,this->rotatedImage,topLeft,bottomRight);
	cv::imwrite("output/rotatedImage.png",this->rotatedImage);
	warp.RotateImage_Ycrop(this->floatingImage,homography,this->rotatedImage,topLeft,bottomRight);		
	cv::imwrite("output/rotatedImage_YCrop.png",this->rotatedImage);

	


	//We have got top left and bottom right points of the rotated image
	//Steps:
	//1. Find the combined area
	//2. Put the rotated and base images there
	//3. Get overlapped areas
	//4. Blend the overlapped area
	//5. put the blended area to the combined area

	Boundry left, top, right, bottom;
	if(topLeft.x<0){
		left.Index=0;
		left.Value=topLeft.x;
	}else{
		left.Index=1;
		left.Value=0;
	}

	if(topLeft.y<0){
		top.Index=0;
		top.Value=topLeft.y;
	}else{
		top.Index=1;
		top.Value=0;
	}

	if(bottomRight.x>this->baseImage.cols){
		right.Index=0;
		right.Value=bottomRight.x;
	}else{
		right.Index=1;
		right.Value=this->baseImage.cols;
	}

	if(bottomRight.y>this->baseImage.rows){
		bottom.Index=0;
		bottom.Value=bottomRight.y;
	}else{
		bottom.Index=1;
		bottom.Value=this->baseImage.rows;
	}	
	cv::Mat stitchedImage(bottom.Value-top.Value+1,right.Value-left.Value+1,CV_8U);
	cv::imwrite("output/stitched.png",stitchedImage);
	
	//paste the rotated and base images in the stitched image
	//we have to define basically 3 regions in the stitched image and 
	//common base region for float image and base image

	cv::Rect floatRegion, baseRegion, commonStitchedRegion, commonBaseRegion, commonFloatRegion;
    
	floatRegion.width=this->rotatedImage.cols;
	floatRegion.height=this->rotatedImage.rows;
	baseRegion.width=this->baseImage.cols;
	baseRegion.height=this->baseImage.rows;

	if(left.Index==0){
		floatRegion.x=0;
		baseRegion.x=abs(left.Value);
		if(top.Index==0){
			floatRegion.y=0;
			baseRegion.y=abs(top.Value);
		}else{
			floatRegion.y=topLeft.y;
			baseRegion.y=0;
		}
	}else{
		floatRegion.x=topLeft.x;
		baseRegion.x=0;
		if(top.Index==0){
			floatRegion.y=0;
			baseRegion.y=abs(top.Value);
		}else{
			floatRegion.y=abs(top.Value);
			baseRegion.y=0;
		}
	}
	this->rotatedImage.copyTo(stitchedImage(floatRegion));
	this->baseImage.copyTo(stitchedImage(baseRegion));
	cv::imwrite("output/o_raw_joined_image.png",stitchedImage);
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





