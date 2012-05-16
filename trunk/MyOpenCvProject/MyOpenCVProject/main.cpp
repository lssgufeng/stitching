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
#include "Utility.h"
#include "Stitching.h"
void testTransformation(cv::Mat& image,double angle,double xTrans, double yTrans);

int main(void)  
{
	//For message display
	char szBuffer[100];

    #pragma region "Input Images"

	/*cv::Mat image2=cv::imread("test6.jpg",0);
	cv::Mat image1=cv::imread("test7.jpg",0);*/

	/*cv::Mat image1=cv::imread("full.png",0);
	cv::Mat image2=cv::imread("center.png",0);	*/
	
	/*cv::Mat image1=cv::imread("knee_1.bmp",0);
	cv::Mat image2=cv::imread("knee_3_moved_rotated.bmp",0);
	*/

	/*cv::Mat image1=cv::imread("Splitted_1.png",0);
	cv::Mat image2=cv::imread("Splitted_rotated_2.png",0);*/
	cv::Mat image1=cv::imread("Original_light.png",0);
	cv::Mat image2=cv::imread("Splitted_rotated_2.png",0);

	/*cv::Mat image1=cv::imread("tl.jpg",0);
	cv::Mat image2=cv::imread("br.jpg",0);*/

	/*cv::Mat image1=cv::imread("mountain.jpg",0);
	cv::Mat image2=cv::imread("mountain1.jpg",0);*/

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


	//Warp warp;
	////
	//cv::Mat rotatedImage;
	//for(int i=0;i<361;i+=10){
	////TestTransformation(image1,i,0,0);
	//	cv::Mat homography(3,3,CV_64F);
	// 	warp.GetCustomHomography(i,240,234,homography);
	//    warp.RotateImage(image1,rotatedImage,homography);
	//	sprintf(szBuffer,"output/rotated_%d.png",i);
	//	
	//	cv::imwrite(szBuffer,rotatedImage);
	//	cv::imshow("Rotated image",rotatedImage);
	//	cv::waitKey(0);
	//}
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
	cv::imwrite("output/o_Image1(keyPoints).bmp",tmpImage);

	cv::drawKeypoints(image2,keyPoints2,tmpImage);
	//displayImage("keypoints2",tmpImage);
	////cv::waitKey(0);
	cv::imwrite("output/o_Image2(keyPoints).bmp",tmpImage);
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
	cv::imwrite("output/o_SymmetryMatches.bmp",tmpImage);

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

	//rotateCenter(image1,homography);
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
	//Utility::DisplayImage("inlier1",tmpImage);
	//cv::waitKey(0);
	cv::imwrite("output/o_Image1(inliers).bmp",tmpImage);

	matching.DrawInliers(points2,inliers,image2,tmpImage);
	//Utility::DisplayImage("inlier2",tmpImage);
	//cv::waitKey(0);
	cv::imwrite("output/o_Image2(inliers).bmp",tmpImage);

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

	/*cv::Mat output;	
	Warp warp;
	
	warp.RotateImage(image1,output,homography);*/

	//Stitching Process starts
	Stitching stitching(image1,image2,homography);
	stitching.Stitch();
	std::getchar();	
}