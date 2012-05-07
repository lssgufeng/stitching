/**
 * @function main
 * @brief main function as entry point. 
 * @author Krishna
 */
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "HarrisDetector.h"
#include "Corners.h"
#include "Matching.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string.h>
#include "MyTimer.h"
#include "Arithmatic.h"
#include "MyFilter.h"
#include "Warp.h"
void displayImage(char* title,cv::Mat& image);
void testTransformation(cv::Mat& image,double angle,double xTrans, double yTrans);
void rotateCenter(cv::Mat image, cv::Mat homography);

int main(void)  
{
	//For message display
	char szBuffer[100];

#pragma region "Input Images"

	cv::Mat image1=cv::imread("Splitted_1.png",0);
	cv::Mat image2=cv::imread("Splitted_Rotated_2.png",0);	
	
	/*cv::Mat image2=cv::imread("knee_1.bmp",0);
	cv::Mat image1=cv::imread("knee_2.bmp",0);
	*/

	/*cv::Mat image1=cv::imread("check1.png",0);
	cv::Mat image2=cv::imread("check2.png",0);*/

	if(!image1.data ||!image2.data){
		printf("Error: Image Not Found!");
		std::getchar();	
	}

#pragma endregion 


	
     
	////alpha beta blending
	//cv::Mat blended;
	//double alpha=0.5;

	//cv::addWeighted(image1,alpha,image2,1.0-alpha,0.0,blended);
	//displayImage("blended",blended);
	//cv::imwrite("blended.bmp",blended);


	/*for(int i=0;i<361;i+=10){
		testTransformation(image1,i,image1.cols/2,image1.rows/2);
	}*/
	//testTransformation(image1,10,0,0);
	
	/*HarrisDetector detector;
	detector.detect(image1);
	std::vector<cv::Point> pts;
	detector.getCorners(pts,0.00001);*/
	cv::Mat tmpImage;
	/*detector.drawOnImage(tmpImage,pts);
	cv::imshow("harris Points",tmpImage);
	cv::waitKey(0);*/	
	

	Corners corner;
	std::vector<cv::KeyPoint> keyPoints1,keyPoints2;	

	//corner.GetSurfFeatures(image1,keyPoints1);
	//corner.GetSurfFeatures(image2,keyPoints2);
	MyTimer timer;
	timer.start("Points detection for image1 started...");
	//corner.GetDynamicAdaptedFeatures_SURF(image1,1500,4700,keyPoints1,130);
	corner.GetSurfFeatures(image1,keyPoints1);
	timer.check();
	timer.start("Points detection for image1 started...");
	//corner.GetDynamicAdaptedFeatures_SURF(image2,1500,4700,keyPoints2,130);
	corner.GetSurfFeatures(image2,keyPoints2);
	timer.check();


	//SD
	/*Arithmatic arithmatic;
	arithmatic.CalculateSD(image1);*/
	//corner.getImageInformation(image1);


	//>>>>>>>>>>>>> DISPLAY
	cv::drawKeypoints(image1,keyPoints1,tmpImage);
	////cv::imshow("keypoints1",tmpImage);
	////cv::waitKey(0);
	//displayImage("KeyPoints1",tmpImage);
	cv::imwrite("o_Image1(keyPoints).bmp",tmpImage);

	cv::drawKeypoints(image2,keyPoints2,tmpImage);
	//displayImage("keypoints2",tmpImage);
	////cv::waitKey(0);
	cv::imwrite("o_Image2(keyPoints).bmp",tmpImage);
	//
	//sprintf(szBuffer, "Key Points1=%i\nKey Point2=%d", keyPoints1.size(),keyPoints2.size());
	//MessageBoxA(NULL,szBuffer,"Key Points Result",MB_OK);	
	//ENDISPLAY   <<<<<<<<<<<<<<<


	Matching matching;
	std::vector<std::vector<cv::DMatch>> matches1,matches2;
	timer.restart("Matching...");
	matching.GetMatchesSurf(image1,image2,keyPoints1,keyPoints2,matches1,matches2);	
	timer.check();
	
	// >>>>>>>>>>>>>>> DISPLAY
	/*sprintf(szBuffer,"image1->image2 matches=%d\nimage2->image1 matches=%d",
		matches1.size(),matches2.size());
	MessageBoxA(NULL,szBuffer,"Matching Result",MB_OK);*/
	// ENDISPLAY <<<<<<<<<<<<<<<<<

	timer.restart("Ratio Test1...");
	int removed1=matching.RatioTest(matches1,0.8);	
	timer.check();
	timer.restart("Ratio Test2...");
	int removed2=matching.RatioTest(matches2,0.8);	
	timer.check();
	
	//>>>>>>>>>>>>>>>>>>DISPLAY
	/*sprintf(szBuffer,"Removed Points\n image1=%d\n image2=%d",
		removed1,removed2);
	MessageBoxA(NULL,szBuffer,"Ratio Test",MB_OK);*/
	//ENDISPLAY  <<<<<<<<<<<<<<<

	std::vector<cv::DMatch> symmetryMatches;
	timer.restart("Symmetry Matches....");
	matching.SymmetryTest(matches1,matches2,symmetryMatches);
	timer.check();

	//>>>>>>>>>>>>>>>>DISPLAY
	matching.DrawMatches(image1,keyPoints1,image2,keyPoints2,symmetryMatches,tmpImage);
	//displayImage("Symmetry Matches",tmpImage);
	////cv::waitKey(0);
	cv::imwrite("o_SymmetryMatches.bmp",tmpImage);

	//sprintf(szBuffer,"Selected Matches=%d",
	//	symmetryMatches.size());
	//MessageBoxA(NULL,szBuffer,"Symmetry Test",MB_OK);
	//ENDISPLAY  


	/*
	std::vector<cv::DMatch> ransacMatches;
	matching.RansacTest(symmetryMatches,keyPoints1,keyPoints2,3.0,0.9,ransacMatches);
	printf("After RANSAC=%d points.",ransacMatches.size());
	*/

	/* Get Top 14 matches */
	//std::nth_element(matches1.begin(),matches1.begin()+24,matches1.end());
	//matches1.erase(matches1.begin()+24,matches1.end());

	cv::Mat imageMatches;	
	std::vector<uchar> inliers;
	cv::Mat homography;
	timer.restart("Homography...");
	homography=matching.GetHomography(symmetryMatches,keyPoints1,keyPoints2,inliers);	
	timer.check();

 
	
	

	//**********DISPLAY

	rotateCenter(image1,homography);
	return 1;
	/*int inliers_count=0;
	for(std::vector<uchar>::const_iterator iterator=inliers.begin();
		iterator!=inliers.end();++iterator){
			if(*iterator){
				++inliers_count;
			}
	}*/

	/*sprintf(szBuffer,"Inliers Count=%d",
		inliers_count);
	MessageBoxA(NULL,szBuffer,"Homography Result",MB_OK);*/

	std::vector<cv::Point2f> points1,points2;
	matching.GetFloatPoints(keyPoints1,keyPoints2,symmetryMatches,points1,points2);
	matching.DrawInliers(points1,inliers,image1,tmpImage);
	displayImage("inlier1",tmpImage);
	//cv::waitKey(0);
	cv::imwrite("o_Image1(inliers).bmp",tmpImage);

	matching.DrawInliers(points2,inliers,image2,tmpImage);
	displayImage("inlier2",tmpImage);
	//cv::waitKey(0);
	cv::imwrite("o_Image2(inliers).bmp",tmpImage);

	//*********
		
	

	//We have homography matrix, now the final task 
	//is to transform image1 on image 2 and stitch together

	std::fstream file;
	file.open("homography.txt",std::ios::app);
	file<<"\nhomography=\n"<<homography;
	printf("value=%f",homography.at<double>(0,2));

	//cv::Mat destination;
	/*cv::warpPerspective(image1,destination,homography,cv::Size(image1.cols*2,image1.rows*2),CV_WARP_FILL_OUTLIERS);
	displayImage("warp",destination);
	cv::imwrite("o_Warp.bmp",destination);*/

	cv::Mat output;
	cv::warpPerspective(image1,output,homography,image1.size());
	displayImage("warp_without",output);

	cv::Point sourceCenter,destCenter;
	sourceCenter=cv::Point(image1.cols/2,image1.rows/2);
	double x=sourceCenter.x,y=sourceCenter.y;
	double Z=1./(homography.at<double>(2,0)*x+
		homography.at<double>(2,1)*y+
		homography.at<double>(2,2));
	double X=(homography.at<double>(0,0)*x+
		homography.at<double>(0,1)*y+
		homography.at<double>(0,2))*Z;
	double Y=(homography.at<double>(1,0)*x+
		homography.at<double>(1,1)*y+
		homography.at<double>(1,2))*Z;
	destCenter=cv::Point(X,Y);
	//Set Values
	homography.at<double>(0,2)+=sourceCenter.x-destCenter.x;
	homography.at<double>(1,2)+=sourceCenter.y-destCenter.y;

	
	cv::warpPerspective(image1,output,homography,image1.size());
	displayImage("warp",output);
	cv::imwrite("o_warped.bmp",output);

}

/***
* Tests the transformation of the image with the supplied parameters
* @image the image to transform
* @angle rotation angle(in degrees)
* @xTrans x translation
* @yTrans y translation
***/
void testTransformation(cv::Mat& image,
	double angle,
	double xTrans, 
	double yTrans){
	cv::Mat t(3,3,CV_64F);
	t=0;
	double PI=3.141592654;
	angle=angle*PI/180;

	t.at<double>(0,0)=cos(angle);
	t.at<double>(1,1)=cos(angle);

	t.at<double>(0,1) = -sin(angle);
	t.at<double>(1,0) = sin(angle);
	
	t.at<double>(0,2) = xTrans;    
	t.at<double>(1,2) = yTrans;
	
	t.at<double>(2,2) = 1;
	t.at<double>(2,0)=t.at<double>(2,1)=0;

	cv::Point sourceCenter,destCenter;
	sourceCenter=cv::Point(image.cols/2,image.rows/2);
	double x=sourceCenter.x,y=sourceCenter.y;
	double Z=1./(t.at<double>(2,0)*x+
		t.at<double>(2,1)*y+
		t.at<double>(2,2));
	double X=(t.at<double>(0,0)*x+
		t.at<double>(0,1)*y+
		t.at<double>(0,2))*Z;
	double Y=(t.at<double>(1,0)*x+
		t.at<double>(1,1)*y+
		t.at<double>(1,2))*Z;
	destCenter=cv::Point(X,Y);

	//Set Values
	t.at<double>(0,2)+=sourceCenter.x-destCenter.x;
	t.at<double>(1,2)+=sourceCenter.y-destCenter.y;

	cv::Mat destination;
	double distance=sqrt((double)(image.rows*image.rows)+(image.cols*image.cols));
	cv::Mat padded(image.rows+2*distance,image.cols+2*distance,CV_8U);
	cv::Mat imageROI=padded(cv::Rect(distance,distance,image.cols,image.rows));
	//image.copyTo(imageROI);
	//displayImage("ROI",padded);
	//cv::imwrite("ROI.bmp",padded);
	cv::warpPerspective(image,destination,t,padded.size(),CV_WARP_FILL_OUTLIERS);
	image.copyTo(imageROI);
	cv::imwrite("transformed.bmp",destination);
	displayImage("transform",destination);		
}

void rotateCenter(cv::Mat image, cv::Mat homography){
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
	homography.at<double>(0,2)+=srcCenter.x-dstCenter.x+25;
	homography.at<double>(1,2)+=srcCenter.y-dstCenter.y+25;

	//Getting new image size
	cv::Mat corners(3,4,CV_64F),dstCorners;
	//top left
	corners.at<double>(0,0)=0;corners.at<double>(1,0)=0;corners.at<double>(2,0)=1;
	corners.at<double>(0,1)=image.cols;corners.at<double>(1,1)=0;corners.at<double>(2,1)=1;
	corners.at<double>(0,2)=image.cols;corners.at<double>(1,2)=image.rows;corners.at<double>(2,2)=1;
	corners.at<double>(0,3)=0;corners.at<double>(1,3)=image.rows;corners.at<double>(2,3)=1;
	
	warp.GetWarpPoints(homography,corners,dstCorners);
	
	double minX,minY,maxX,maxY;
	//Getting the size of the warped image
	for(int i=0;i<dstCorners.rows-1;i++){
		double* row=dstCorners.ptr<double>(i);
		cv::Mat tmp(1,dstCorners.cols,CV_64F);
		for(int j=0;j<dstCorners.cols;j++){
			tmp.at<double>(0,j)=*row++;
		}
		cv::minMaxLoc(tmp,&minX,maxX);

	}

	cv::warpPerspective(image,destination,homography,cv::Size(image.cols+50,image.rows+50));
	displayImage("warped",destination);
}

void displayImage(char* title, cv:: Mat& image){
	cv::Mat tmpImage;	
	cv::resize(image,tmpImage,image.size());
	cv::imshow(title,tmpImage);
	cv::waitKey(0);
}
