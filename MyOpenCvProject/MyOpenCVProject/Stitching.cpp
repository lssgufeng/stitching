#include "Stitching.h"
#include "Warp.h"

Stitching::Stitching(cv::Mat floatingImage,
	cv::Mat baseImage,
	cv::Mat homography){
		this->floatImage=floatImage;
		this->baseImage=baseImage;
		this->homography=homography;		
}

Stitching::~Stitching(){
	this->floatImage.deallocate();
	this->baseImage.deallocate();
	this->homography.deallocate();
}
void Stitching::Stitch(){
	//1.Get the corners
	cv::Point baseCorners[4],floatingCorners[4];
	baseCorners[0]=cv::Point(0,0);
	baseCorners[1]=cv::Point(this->baseImage.cols,0);
	baseCorners[2]=cv::Point(this->baseImage.cols,this->baseImage.rows);
	baseCorners[3]=cv::Point(0,this->baseImage.rows);
	Warp warp;
	warp.TransformCorners(baseCorners,floatingCorners,&this->homography);

	for(int i=0;i<4;i++){
		printf("%f",floatingCorners[i]);
	}

	//2.Calculate the size of the combined image
	//3.Get LEFT ROI
	//4.Get RIGHT ROI
	//5.Get TOP ROI
	//6.Get BOTTOM ROI
	//7.Join ROIS to the base image
	//8.Get overlapped ROI and blend
	//9.Copy the nonoverlaped base image content
}


