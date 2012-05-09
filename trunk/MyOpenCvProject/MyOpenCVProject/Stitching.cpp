#include "Stitching.h"
#include "Warp.h"

Stitching::Stitching(cv::Mat floatingImage,
	cv::Mat baseImage,
	cv::Mat homography){
		this->floatingImage=floatingImage;
		this->baseImage=baseImage;
		this->homography=homography;		
}

Stitching::~Stitching(){
}
void Stitching::Stitch(){
	//1.Get the new transformed corners and rotated image
	cv::Point baseCorners[4],floatingCorners[4];
	baseCorners[0]=cv::Point(0,0);
	baseCorners[1]=cv::Point(this->floatingImage.cols,0);
	baseCorners[2]=cv::Point(this->floatingImage.cols,this->floatingImage.rows);
	baseCorners[3]=cv::Point(0,this->floatingImage.rows);
	Warp warp;	
	//double tic=cv::getTickCount();
	warp.TransformCorners(baseCorners,floatingCorners,homography);
	//#### Testing for performance
	//printf("Time Taken=%f Seconds",(cv::getTickCount()-tic)/cv::getTickFrequency());
	/*for(int i=0;i<4;i++){
		printf("\n1st  point %d x=%d y=%d",i,floatingCorners[i].x,floatingCorners[i].y);
	}*/
	warp.RotateImage(floatingImage,this->rotatedImage,homography);
	/*
	tic=cv::getTickCount();
	for(int i=0;i<4;i++){
		warp.TransformPoint(*(baseCorners+i),*(floatingCorners+i),&homography);
	}
	printf("Time Taken=%f Seconds",(cv::getTickCount()-tic)/cv::getTickFrequency());
	for(int i=0;i<4;i++){
		printf("\nPoint %d X=%d Y=%d",i,floatingCorners[i].x,floatingCorners[i].y);
	}*/


	//2.Calculate the size of the combined image
	//get minimum x, maximum x, minimum y, maximum y	
	Boundry left,top,right,bottom;
	//Initialisation
	left.Index=0;right.Index=0;left.Value=right.Value=floatingCorners[0].x;
	top.Index=0; bottom.Index=0; top.Value=bottom.Value=floatingCorners[0].y;
	for(int i=0;i<4;i++){
		//Get maximum and minimum x values
		if(floatingCorners[i].x<baseCorners[i].x){
			if(floatingCorners[i].x<left.Value){
				left.Index=0;
				left.Value=floatingCorners[i].x;
			}
			if(baseCorners[i].x>right.Value){
				right.Index=1;
				right.Value=baseCorners[i].x;
			}
		}else{
			if(baseCorners[i].x<left.Value){
				left.Index=1;
				left.Value=baseCorners[i].x;
			}
			if(floatingCorners[i].x>right.Value){
				right.Index=0;
				right.Value=floatingCorners[i].x;
			}
		}

		//Get maximum and minimum y values
		if(floatingCorners[i].y<baseCorners[i].y){
			if(floatingCorners[i].y<top.Value){
				top.Index=0;
				top.Value=floatingCorners[i].y;
			}
			if(baseCorners[i].y>bottom.Value){
				bottom.Index=1;
				bottom.Value=baseCorners[i].y;
			}
		}else{
			if(baseCorners[i].y<top.Value){
				top.Index=1;
				top.Value=baseCorners[i].y;
			}
			if(floatingCorners[i].y>bottom.Value){
				bottom.Index=0;
				bottom.Value=floatingCorners[i].y;
			}
		}
	}
	printf("Combined boundary: left=%d,top=%d,bottom=%d, right=%d",
		left.Value,top.Value,bottom.Value,right.Value);


	//next method to calculate
	int image1Left=floatingCorners[0].x, image1Top=floatingCorners[0].y,
		image1Right=floatingCorners[0].x, image1Bottom=image1Top=floatingCorners[0].y;
	int image2Left=0,image2Top=0,image2Right=baseImage.cols,image2Bottom=baseImage.rows;
	for(int i=0;i<4;i++){
		if(floatingCorners[i].x<image1Left){
			image1Left=floatingCorners[i].x;
		}
		if(floatingCorners[i].x>image1Right){
			image1Right=floatingCorners[i].x;
		}
		if(floatingCorners[i].y<image1Top){
			image1Top=floatingCorners[i].y;
		}
		if(floatingCorners[i].y>image1Bottom){
			image1Bottom=floatingCorners[i].y;
		}
	}

	if(image1Left<image2Left){
		left.Index=0;
		left.Value=image1Left;
	}else{
		left.Index=1;
		left.Value=image2Left;
	}
	if(image1Top<image2Top){
		top.Index=0;
		top.Value=image1Top;
	}else{
		top.Index=1;
		top.Value=image2Top;
	}
	if(image1Right>image2Right){
		right.Index=0;
		right.Value=image1Right;
	}else{
		right.Index=1;
		right.Value=image2Right;
	}
	if(image1Bottom>image2Bottom){
		bottom.Index=0;
		bottom.Value=image1Bottom;
	}else{
		bottom.Index=1;
		bottom.Value=image2Bottom;
	}


	
	

	printf("Combined boundary: left=%d,top=%d,bottom=%d, right=%d",
		left.Value,top.Value,bottom.Value,right.Value);

	//3.Get LEFT ROI
	//4.Get RIGHT ROI
	//5.Get TOP ROI
	//6.Get BOTTOM ROI
	//7.Join ROIS to the base image
	//8.Get overlapped ROI and blend
	//9.Copy the nonoverlaped base image content
}



