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
	log->Write("This is test");
	log->Write("Left:Index=%d,value=%d\nTop:Index=%d,value=%d\nRight:Index=%d,value=%d,Bottom:Index=%d,value=%d\n",
		left.Index,left.Value,top.Index,top.Value,right.Index,right.Value,bottom.Index,bottom.Value);
	cv::Mat stitchedImage(bottom.Value-top.Value+1,right.Value-left.Value+1,CV_16U);
	cv::imwrite("output/stitched.png",stitchedImage);
	//paste the rotated and base images in the stitched image


	


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





