#include "Warp.h"
#include "Utility.h"
#include <iostream>
#define PI 3.141592653589793238462643383279502884197

Warp::Warp(){
}
Warp::~Warp(){
}
void Warp::GetWarpPoints(cv::Mat& srcPts,
		cv::Mat& dstPts,
		cv::Mat& homography){
			double tic=cv::getTickCount();
			//check feasibility
			if(homography.cols==srcPts.rows){
				dstPts=homography*srcPts;
				for(int i=0;i<dstPts.rows;i++){
					double* data=dstPts.ptr<double>(i);
					double* scale=dstPts.ptr<double>(dstPts.rows-1);
					for(int j=0;j<dstPts.cols;j++){
						*data++=*data / *scale++;
					}			
				}
			}
			printf("\nGetWarpPoints took %f seconds\n",(cv::getTickCount()-tic)/cv::getTickFrequency());
}


void Warp::GetWarpPoint(cv::Mat& homography,
		cv::Point& point,
		cv::Point& dstPoint){
			double tic=cv::getTickCount();
			cv::Mat tmpPointSrc(3,1,CV_64F),tmpPointDst;
			
			tmpPointSrc.at<double>(0,0)=point.x;
			tmpPointSrc.at<double>(0,1)=point.y;
			tmpPointSrc.at<double>(0,2)=1;
			GetWarpPoints(homography,tmpPointSrc,tmpPointDst);
			dstPoint=cv::Point(tmpPointDst.at<double>(0,0),
				tmpPointDst.at<double>(0,1));
			printf("GetWarpPoint took %f seconds\n",(cv::getTickCount()-tic)/cv::getTickFrequency());
}

void Warp::TransformPoint(const cv::Point pointToTransform,
		cv::Point& outputPoint,
		const cv::Mat* homography) {
			double tic=cv::getTickCount();
			double coordinates[3] = {pointToTransform.x, pointToTransform.y, 1};
			cv::Mat originVector = cv::Mat(3, 1, CV_64F, coordinates);
			cv::Mat transformedVector = cv::Mat(3, 1, CV_64F, coordinates);
			//cv::MatMul(matrix, &originVector, &transformedVector);
			transformedVector=*homography*originVector;
			outputPoint = cv::Point((int)transformedVector.at<double>(0,0)/ transformedVector.at<double>(2, 0),
				(int)(transformedVector.at<double>(1,0)/ transformedVector.at<double>(2, 0)));
			printf("TransformPoint took %f seconds\n",(cv::getTickCount()-tic)/cv::getTickFrequency());
}

void Warp::TransformCorners(const cv::Point* corners,
	cv::Point* outputCorners,
	const cv::Mat homography){
		for(int i=0;i<4;i++){
			TransformPoint(*(corners+i),*(outputCorners+i),&homography);
		}	
}

void Warp::RotateImage(cv::Mat image,cv::Mat& outputImage,cv::Mat homography){
	std::cout<<std::endl<<"homography="<<std::endl<<homography<<std::endl;
	double tic=cv::getTickCount();
	//setting the translation to 
	cv::Point srcCenter,dstCenter;
	srcCenter=cv::Point(image.cols/2,image.rows/2);	
	this->TransformPoint(srcCenter,dstCenter,&homography);
	homography.at<double>(0,2)+=srcCenter.x-dstCenter.x;
	homography.at<double>(1,2)+=srcCenter.y-dstCenter.y;
	//Getting new image size
	cv::Mat corners(3,4,CV_64F),dstCorners;
	//top left
	corners.at<double>(0,0)=0;corners.at<double>(1,0)=0;corners.at<double>(2,0)=1;
	corners.at<double>(0,1)=image.cols;corners.at<double>(1,1)=0;corners.at<double>(2,1)=1;
	corners.at<double>(0,2)=image.cols;corners.at<double>(1,2)=image.rows;corners.at<double>(2,2)=1;
	corners.at<double>(0,3)=0;corners.at<double>(1,3)=image.rows;corners.at<double>(2,3)=1;
	
	tic=cv::getTickCount();
	this->GetWarpPoints(corners,dstCorners,homography);
	printf("GetWarpPoints took %f seconds",(cv::getTickCount()-tic)/cv::getTickFrequency());
	std::cout<<"corners="<<std::endl<<" "<<corners<<std::endl<<std::endl;
	std::cout<<"corners="<<std::endl<<" "<<dstCorners<<std::endl<<std::endl;
	double minX,minY,maxX,maxY;
	//Getting the size of the warped image
	for(int i=0;i<dstCorners.rows-1;i++){
		double* row=dstCorners.ptr<double>(i);
		cv::Mat tmp(1,dstCorners.cols,CV_64F);
		for(int j=0;j<dstCorners.cols;j++){
			tmp.at<double>(0,j)=*row++;
		}
		if(i==0)
			cv::minMaxLoc(tmp,&minX,&maxX);
		else if(i==1)
			cv::minMaxLoc(tmp,&minY,&maxY);
		else break;
	}
	double newWidth=maxX-minX;
	double newHeight=maxY-minY;
	int shiftX=(newWidth-image.cols)/2;
	int shiftY=(newHeight-image.rows)/2;  
	homography.at<double>(0,2)+=shiftX;
	homography.at<double>(1,2)+=shiftY;
	homography.at<double>(2,0)=0;
	homography.at<double>(2,1)=0;


	cv::warpPerspective(image,outputImage,homography,cv::Size(newWidth,newHeight));
	printf("Rotating took %f seconds",(cv::getTickCount()-tic)/cv::getTickFrequency());
	//Utility::DisplayImage("warped",outputImage);
	cv::imwrite("original.bmp",image);
	cv::imwrite("warped.bmp",outputImage);
}

void Warp::TestTransformation(cv::Mat& image,
	double angle,
	double xTrans, 
	double yTrans){
	cv::Mat t(3,3,CV_64F);
	t=0;
	angle=angle*PI/180;

	t.at<double>(0,0)=cos(angle);
	t.at<double>(1,1)=cos(angle);

	t.at<double>(0,1) = -sin(angle);
	t.at<double>(1,0) = sin(angle);
	
	t.at<double>(0,2) = xTrans;    
	t.at<double>(1,2) = yTrans;
	
	t.at<double>(2,2) = 1;
	t.at<double>(2,0)=t.at<double>(2,1)=0;	

	
	cv::Mat destination;	
	cv::warpPerspective(image,destination,t,image.size());
	cv::imshow("Transform Result",destination);
	cv::waitKey(0);
}

void Warp::GetCustomHomography(double angle, 
		double xTrans, 
		double yTrans,
		cv::Mat homography){
			angle*=PI/180;
			homography.at<double>(0,0)=homography.at<double>(1,1)=cos(angle);
			homography.at<double>(0,1)=-sin(angle);
			homography.at<double>(1,0)=sin(angle);
			homography.at<double>(0,2) = xTrans;    
			homography.at<double>(1,2) = yTrans;
			homography.at<double>(2,2) = 1;
			homography.at<double>(2,0)=homography.at<double>(2,1)=0;	
}




