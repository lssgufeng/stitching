#include "Stitching.h"

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
	//2.Calculate the size of the combined image
	//3.Get LEFT ROI
	//4.Get RIGHT ROI
	//5.Get TOP ROI
	//6.Get BOTTOM ROI
	//join ROIS
}


