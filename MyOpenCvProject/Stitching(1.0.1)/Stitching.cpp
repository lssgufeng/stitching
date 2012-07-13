//#include "Stitching.h"
#include "Warp.h"
#include "AlphaBlender.h"
#include "Corners.h"
#include "Matching.h"
#include "Utility.h"


Stitching::Stitching(cv::Mat floatingImage,
	cv::Mat baseImage,int direction){
		this->floatingImage=floatingImage;
		this->baseImage=baseImage;	
		this->direction=direction;
}

Stitching::~Stitching(){
}

cv::Mat Stitching::Stitch(){
	cv::Mat homography;	
	int floatingHeight=this->floatingImage.rows;
	int floatingWidth=this->floatingImage.cols;
	int baseHeight=this->baseImage.rows;
	int baseWidth=this->baseImage.cols;
	float scale=1.0;
	int standardSize=2000;
	if(floatingWidth>standardSize){
		scale=(float)standardSize/floatingWidth;
	}else if(floatingHeight>standardSize){
		scale=(float)standardSize/floatingHeight;
	}
	cv::Mat floatingImageResized((int)floatingHeight*scale,(int)floatingWidth*scale,this->floatingImage.type());
	cv::Mat baseImageResized((int)baseHeight*scale,(int)baseWidth*scale,this->baseImage.type());
	cv::resize(this->floatingImage,floatingImageResized,floatingImageResized.size());
	cv::resize(this->baseImage,baseImageResized,baseImageResized.size());
	
	
	cv::Mat cropFloatingImage(floatingImageResized.rows,floatingImageResized.cols,CV_16U);
	cv::Mat cropBaseImage(baseImageResized.rows,baseImageResized.cols,CV_16U);
	bool success=false;
	//int effectiveLength=600;
	//Horizontal
	if(this->direction==0){	
		floatingImageResized.colRange(floatingImageResized.cols/2,
			floatingImageResized.cols).copyTo(cropFloatingImage.colRange(floatingImageResized.cols/2,
			floatingImageResized.cols));
		baseImageResized.colRange(0,baseImageResized.cols/2).copyTo(cropBaseImage.colRange(0,
			baseImageResized.cols/2));
		success=calculateHomography(cropFloatingImage,
			cropBaseImage,homography);	
		if(!success){
			baseImageResized.colRange(baseImageResized.cols/4,3*baseImageResized.cols/4).copyTo(cropBaseImage.colRange(baseImageResized.cols/4,3*baseImageResized.cols/4));
			success=calculateHomography(cropFloatingImage,
				cropBaseImage,homography);
			if(!success){
				baseImageResized.colRange(baseImageResized.cols/2,baseImageResized.cols).copyTo(cropBaseImage.colRange(baseImageResized.cols/2,baseImageResized.cols));
				success=calculateHomography(cropFloatingImage,
				cropBaseImage,homography);
			}
		}
		
    //Vertical
	}else if(this->direction==1){
		floatingImageResized.rowRange(floatingImageResized.rows/2,
			floatingImageResized.rows).copyTo(cropFloatingImage.rowRange(floatingImageResized.rows/2,
			floatingImageResized.rows));
		baseImageResized.rowRange(0,baseImageResized.rows/2).copyTo(cropBaseImage.rowRange(0,baseImageResized.rows/2));
		success=calculateHomography(cropFloatingImage,
			cropBaseImage,homography);			
		if(!success){
			baseImageResized.rowRange(baseImageResized.rows/4,3*baseImageResized.rows/4).copyTo(cropBaseImage.rowRange(baseImageResized.rows/4,3*baseImageResized.rows/4));
			success=calculateHomography(cropFloatingImage,cropBaseImage,homography);
			if(!success){
				baseImageResized.rowRange(3*baseImageResized.rows/4,
					baseImageResized.rows).copyTo(cropBaseImage.rowRange(3*baseImageResized.rows/4,
					baseImageResized.rows));
				success=calculateHomography(cropFloatingImage,
					cropBaseImage,homography);
			}
		}
	}else{
		success=calculateHomography(floatingImageResized,
			baseImageResized,homography);
	}
	homography.at<double>(0,2)*=1/scale;
		homography.at<double>(1,2)*=1/scale;
	Warp warp;

	cv::Point topLeft, bottomRight;
	if(this->direction==0){
		warp.RotateImage_Xcrop(this->floatingImage,homography,this->rotatedImage,topLeft,bottomRight);		
	}else if(this->direction==1){
		warp.RotateImage_Ycrop(this->floatingImage,homography,this->rotatedImage,topLeft,bottomRight);
	}else{
		warp.RotateImage(this->floatingImage,homography,this->rotatedImage,topLeft,bottomRight);
	}

		
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
	cv::Mat stitchedImage(bottom.Value-top.Value+1,right.Value-left.Value+1,CV_16U);
	
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


			//Common Region
			int commonWidth=cv::min(this->rotatedImage.cols-abs(topLeft.x),this->baseImage.cols);
			int commonHeight=cv::min(this->rotatedImage.rows-abs(topLeft.y),this->baseImage.rows);

			commonFloatRegion.width=commonBaseRegion.width=commonStitchedRegion.width=commonWidth;
			commonFloatRegion.height=commonBaseRegion.height=commonStitchedRegion.height=commonHeight;
			
			commonFloatRegion.x=abs(topLeft.x);
			commonFloatRegion.y=abs(topLeft.y);

			commonBaseRegion.x=0;
			commonBaseRegion.y=0;

			commonStitchedRegion.x=abs(topLeft.x);
			commonStitchedRegion.y=abs(topLeft.y);
		}else{
			floatRegion.y=topLeft.y;
			baseRegion.y=0;

			//Common Region
			int commonWidth=cv::min(this->rotatedImage.cols-abs(topLeft.x),this->baseImage.cols);
			int commonHeight=cv::min(this->rotatedImage.rows,this->baseImage.rows-topLeft.y);

			commonFloatRegion.width=commonBaseRegion.width=commonStitchedRegion.width=commonWidth;
			commonFloatRegion.height=commonBaseRegion.height=commonStitchedRegion.height=commonHeight;
			
			commonFloatRegion.x=abs(topLeft.x);
			commonFloatRegion.y=0;

			commonBaseRegion.x=0;
			commonBaseRegion.y=abs(topLeft.y);

			commonStitchedRegion.x=abs(topLeft.x);
			commonStitchedRegion.y=topLeft.y;
		}
	}else{
		floatRegion.x=topLeft.x;
		baseRegion.x=0;
		if(top.Index==0){
			floatRegion.y=0;
			baseRegion.y=abs(top.Value);

			//Common Region
			int commonWidth=cv::min(this->rotatedImage.cols,this->baseImage.cols-topLeft.x);
			int commonHeight=cv::min(this->rotatedImage.rows-abs(topLeft.y),this->baseImage.rows);

			commonFloatRegion.width=commonBaseRegion.width=commonStitchedRegion.width=commonWidth;
			commonFloatRegion.height=commonBaseRegion.height=commonStitchedRegion.height=commonHeight;
			
			commonFloatRegion.x=0;
			commonFloatRegion.y=abs(topLeft.y);

			commonBaseRegion.x=topLeft.x;
			commonBaseRegion.y=0;

			commonStitchedRegion.x=topLeft.x;
			commonStitchedRegion.y=abs(topLeft.y);

		}else{
			floatRegion.y=topLeft.y;
			baseRegion.y=0;

			//Common Region
			int commonWidth=cv::min(this->rotatedImage.cols,this->baseImage.cols-topLeft.x);
			int commonHeight=cv::min(this->rotatedImage.rows,this->baseImage.rows-topLeft.y);

			commonFloatRegion.width=commonBaseRegion.width=commonStitchedRegion.width=commonWidth;
			commonFloatRegion.height=commonBaseRegion.height=commonStitchedRegion.height=commonHeight;
			
			commonFloatRegion.x=0;
			commonFloatRegion.y=0;

			commonBaseRegion.x=topLeft.x;
			commonBaseRegion.y=topLeft.y;

			commonStitchedRegion.x=topLeft.x;
			commonStitchedRegion.y=topLeft.y;
		}
	}
	this->rotatedImage.copyTo(stitchedImage(floatRegion));
	this->baseImage.copyTo(stitchedImage(baseRegion));
	cv::imwrite("output/o_raw_joined_image.png",stitchedImage);

	AlphaBlender alphaBlender;
	cv::Mat result=cv::Mat(commonFloatRegion.height,commonFloatRegion.width,CV_16U);
	alphaBlender.blend(this->rotatedImage(commonFloatRegion),
		this->baseImage(commonBaseRegion),left,top,right,bottom,result);
	result.copyTo(stitchedImage(commonStitchedRegion));

	cv::imwrite("output/o_stitched_alpha.png",stitchedImage);
	//cv::imshow("stitchedImage_alpha",stitchedImage);

	/*LaplacianBlender blender(this->rotatedImage(commonFloatRegion),this->baseImage(commonBaseRegion));
	cv::Mat outputImage(commonFloatRegion.height,commonFloatRegion.width,CV_16U);
	outputImage= blender.blend(left,top,right,bottom);
	outputImage.copyTo(stitchedImage(commonStitchedRegion));
	cv::imwrite("output/o_stitched_pyr.png",stitchedImage);*/
	return stitchedImage;
}

bool Stitching::calculateHomography(cv::Mat image1,cv::Mat image2,cv::Mat& homography){
	Corners corner;
	std::vector<cv::KeyPoint> keyPoints1,keyPoints2;	
	cv::Mat image1_8bit,image2_8bit;
	image1.convertTo(image1_8bit,CV_8U,1./256);
	image2.convertTo(image2_8bit,CV_8U,1./256);

	cv::GaussianBlur(image1_8bit,image1_8bit,cv::Size(11,11),4);
	cv::GaussianBlur(image2_8bit,image2_8bit,cv::Size(11,11),4);

	corner.GetSurfFeatures(image1_8bit,keyPoints1);
	corner.GetSurfFeatures(image2_8bit,keyPoints2);

	cv::Mat tmpImage;
	/*cv::drawKeypoints(image1_8bit,keyPoints1,tmpImage);
	cv::imwrite("output/o_Image1(keyPoints).bmp",tmpImage);
	cv::drawKeypoints(image2_8bit,keyPoints2,tmpImage);
	cv::imwrite("output/o_Image2(keyPoints).bmp",tmpImage);
	*/



	Matching matching;
	std::vector<std::vector<cv::DMatch>> matches1,matches2;
	matching.GetMatchesSurf(image1_8bit,image2_8bit,keyPoints1,keyPoints2,matches1,matches2);	
	int removed1=matching.RatioTest(matches1,0.8);	
	int removed2=matching.RatioTest(matches2,0.8);	
	std::vector<cv::DMatch> symmetryMatches;
	matching.SymmetryTest(matches1,matches2,symmetryMatches);
	
	/*matching.DrawMatches(image1_8bit,keyPoints1,image2_8bit,keyPoints2,symmetryMatches,tmpImage);
	cv::imwrite("output/o_SymmetryMatches.bmp",tmpImage);
	*/

	cv::Mat imageMatches;	
	std::vector<uchar> inliers;
	homography=matching.GetHomography(symmetryMatches,keyPoints1,keyPoints2,inliers);	
	std::vector<cv::Point2f> points1,points2;
	matching.GetFloatPoints(keyPoints1,keyPoints2,symmetryMatches,points1,points2);
	/*matching.DrawInliers(points1,inliers,image1,tmpImage);
	cv::imwrite("output/o_Image1(inliers).png",tmpImage);
	*/
	
	int inliers_count=0;
	for(std::vector<uchar>::const_iterator iterator=inliers.begin();
		iterator!=inliers.end();++iterator){
			if(*iterator){
				++inliers_count;
			}
	}

	if(inliers_count<6){
		return false;
	}

	/*
	matching.DrawInliers(points2,inliers,image2,tmpImage);
	cv::imwrite("output/o_Image2(inliers).png",tmpImage);
	*/
	
	return true;
}


/** This is a method copied from somewhere**/
void Stitching::stich(cv::Mat base, 
	cv::Mat target,cv::Mat homography, cv::Mat& panorama){
	cv::Mat corners1(1, 4,CV_32F);
	cv:: Mat corners2(1,4,CV_32F);
	cv::Mat corners(1,4,CV_32F);
	vector<cv::Mat> planes;


	
	/* compute corners 
	of warped image
	*/
	corners1.at<float>(0,0)=0;
	corners2.at<float>(0,0)=0;
	corners1.at<float>(0,1)=0;
	corners2.at<float>(0,1)=target.rows;
	corners1.at<float>(0,2)=target.cols;
	corners2.at<float>(0,2)=0;
	corners1.at<float>(0,3)=target.cols;
	corners2.at<float>(0,3)=target.rows;

	planes.push_back(corners1);
	planes.push_back(corners2);

	cv::merge(planes,corners);

	perspectiveTransform(corners, corners, homography);

	/*compute size of resulting 
	image and allocate memory
	*/
	double x_start = cv::min( cv::min( (double)corners.at<cv::Vec2f>(0,0)[0], (double)corners.at<cv::Vec2f> (0,1)[0]),0.0);
	double x_end   = cv::max( cv::max( (double)corners.at<cv::Vec2f>(0,2)[0], (double)corners.at<cv::Vec2f>(0,3)[0]), (double)base.cols);
	double y_start = cv::min( cv::min( (double)corners.at<cv::Vec2f>(0,0)[1], (double)corners.at<cv::Vec2f>(0,2)[1]), 0.0);
	double y_end   = cv::max( cv::max( (double)corners.at<cv::Vec2f>(0,1)[1], (double)corners.at<cv::Vec2f>(0,3)[1]), (double)base.rows);

	/*Creating image
	with same channels, depth
	as target
	and proper size
	*/
	panorama.create(cv::Size(x_end - x_start + 1, y_end - y_start + 1), target.depth());
	planes.clear();


	cv::Mat panorama1, panorama2;
	panorama1=panorama.clone();
	panorama2=panorama.clone();

	/*Planes should
	have same n.channels
	as target
	*/
	for (int i=0;i<target.channels();i++){
		planes.push_back(panorama);
	}

	cv::merge(planes,panorama);
	// create translation matrix in order to copy both images to correct places
	cv::Mat T;
	T=cv::Mat::zeros(3,3,CV_64F);
	T.at<double>(0,0)=1;
	T.at<double>(1,1)=1;
	T.at<double>(2,2)=1;
	T.at<double>(0,2)=-x_start;
	T.at<double>(1,2)=-y_start;

	//copy base image to correct position within output image
	cv::warpPerspective(base, panorama1, T,panorama.size(),cv::INTER_LINEAR| CV_WARP_FILL_OUTLIERS);
	cv::imwrite("panorama1.png",panorama1);
	// change homography to take necessary translation into account
	cv::gemm(T, homography,1,T,0,T);
	// warp second image and copy it to output image
	cv::warpPerspective(target,panorama2, T, panorama.size(),cv::INTER_LINEAR);
	cv::imwrite("panorama2.png",panorama2);

	AlphaBlender blender;
	panorama=blender.blend(panorama1,panorama2,panorama);
	cv::imwrite("panorama.png",panorama);

	 //tidy
	corners.release();
	T.release();
}





