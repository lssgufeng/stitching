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
        cv::Mat tmpPointSrc(3,1,CV_64F),tmpPointDst;
		
		tmpPointSrc.at<double>(0,0)=point.x;
		tmpPointSrc.at<double>(0,1)=point.y;
		tmpPointSrc.at<double>(0,2)=1;
		GetWarpPoints(homography,tmpPointSrc,tmpPointDst);
		dstPoint=cv::Point(tmpPointDst.at<double>(0,0),
			tmpPointDst.at<double>(0,1));
}