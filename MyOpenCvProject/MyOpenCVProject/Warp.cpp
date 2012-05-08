#include "Warp.h"

void Warp::GetWarpPoints(cv::Mat& homography,
	cv::Mat& srcPoints,
	cv::Mat& dstPoints){
		//check feasibility
		if(homography.cols==srcPoints.rows){
			dstPoints=homography*srcPoints;
			for(int i=0;i<dstPoints.rows;i++){
				double* data=dstPoints.ptr<double>(i);
				double* scale=dstPoints.ptr<double>(dstPoints.rows-1);
				for(int j=0;j<dstPoints.cols;j++){
                     *data++=*data / *scale++;
				}
			}
		}
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

void Warp::RotateImage(cv::Mat image,cv::Mat homography){
	double tic=cv::getTickCount();
	//setting the translation to 
	cv::Mat destination;
	cv::Point srcCenter,dstCenter;
	srcCenter=cv::Point(image.cols/2,image.rows/2);
	//double x=(double)srcCenter.x,y=(double)srcCenter.y;
	//double Z=1.0/(homography.at<double>(2,0)*x+homography.at<double>(2,1)*y+homography.at<double>(2,2));
	//now get the warped points
	//double X=(homography.at<double>(0,0)*x+homography.at<double>(0,1)*y+homography.at<double>(0,2))*Z;
	//double Y=(homography.at<double>(1,0)*x+homography.at<double>(1,1)*y+homography.at<double>(1,2))*Z;
	//dstCenter=cv::Point(X,Y);
	Warp warp;
	warp.GetWarpPoint(homography,srcCenter,dstCenter);
	homography.at<double>(0,2)+=srcCenter.x-dstCenter.x;
	homography.at<double>(1,2)+=srcCenter.y-dstCenter.y;
	//Getting new image size
	cv::Mat corners(3,4,CV_64F),dstCorners;
	//top left
	corners.at<double>(0,0)=0;corners.at<double>(1,0)=0;corners.at<double>(2,0)=1;
	corners.at<double>(0,1)=image.cols;corners.at<double>(1,1)=0;corners.at<double>(2,1)=1;
	corners.at<double>(0,2)=image.cols;corners.at<double>(1,2)=image.rows;corners.at<double>(2,2)=1;
	corners.at<double>(0,3)=0;corners.at<double>(1,3)=image.rows;corners.at<double>(2,3)=1;
	
	warp.GetWarpPoints(homography,corners,dstCorners);

	cv::Point point;
	warp.GetWarpPoint(homography,cv::Point(12,12),point);
    printf("1st method:x=%d,y=%d",point.x,point.y);
	warp.TransformPoint(cv::Point(12,12),point,&homography);
	printf("2nd method:x=%d,y=%d",point.x,point.y);

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


	cv::warpPerspective(image,destination,homography,cv::Size(newWidth,newHeight));
	printf("Rotating took %f seconds",(cv::getTickCount()-tic)/cv::getTickFrequency());
	displayImage("warped",destination);
	cv::imwrite("warped.bmp",destination);
}