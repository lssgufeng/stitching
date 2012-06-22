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

int main(void)  

{
	//For message display
	char szBuffer[100];

    #pragma region "Input Images"
	




	/*char* path2="C:/Users/krishna/Google Drive/images/test3.jpg";
	char* path1="C:/Users/krishna/Google Drive/images/test4.jpg";*/

	char* path1="C:/Users/krishna/Google Drive/images/16bit/house1.png";
	char* path2="C:/Users/krishna/Google Drive/images/16bit/house2.png";
	


	//char* path1="C:/Users/krishna/Google Drive/images/real/angle/16bit/Test_Stitching_Thorax_middle_66_8_C_16.png";
	//char* path2="C:/Users/krishna/Google Drive/images/real/angle/16bit/Test_Stitching_Thorax_oben10°_66_16_C_16.png";

	/*char* path2="C:/Users/krpaudel/Google Drive/images/real/Parallel/16bit/";
	char* path1="C:/Users/krpaudel/Google Drive/images/real/Parallel/16bit/";*/


	
	
	//printf("path1=%s",path1);printf("path2=%s",path2);
	//printf("path1=%s",path1);printf("path2=%s",path2);

	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	if(!image1.data ||!image2.data){
		printf("Error: Image Not Found!");
		std::getchar();	
		exit(0);
	}

	if(image1.depth()!=CV_16U || image2.depth()!=CV_16U){
		printf("\nThis application need 16 bits images to work!");
		std::getchar();
		exit(0);
	}

	/*Corners corners;

	cv::Mat cornerFeature=image1.clone();
	cv::preCornerDetect(image1,cornerFeature,3);
	cv::imshow("CornerFeature.png",cornerFeature);
	cv::waitKey(0);
	double info1=corners.getImageInformation(image1);
	double info2=corners.getImageInformation(image2);
	printf("Image 1 information=%f image2 Info=%f",info1,info2);*/

	/*cv::Mat image1_8bit,image2_8bit;
	image1.convertTo(image1_8bit,CV_8U,1./256);
	image2.convertTo(image2_8bit,CV_8U,1./256);*/


	/*cv::imshow("Image1",image1_8bit);
	cv::waitKey(0);
	cv::imshow("Image1",image2_8bit);
	cv::waitKey(0);

	cv::imwrite("image1.png",image1);*/

	//std::cout<<image1;

	

	std::cout<<"Channels="<<image1.channels()<<"\tType="<<image1.type()<<"\tDepth="<<image1.depth();;

    #pragma endregion 
	for(int i=0;i<image1.rows;i++){
		for(int j=0;j<image1.cols;j++){
			if(image1.at<ushort>(i,j)==0){
				image1.at<ushort>(i,j)=1;
			}			
		}
	}
	for(int i=0;i<image2.rows;i++){
		for(int j=0;j<image2.cols;j++){
			if(image2.at<ushort>(i,j)==0){
				image2.at<ushort>(i,j)=1;
			}			
		}
	}
	

	////alpha beta blending
	//cv::Mat blended;
	//double alpha=0.5;

	//cv::addWeighted(image1,alpha,image2,1.0-alpha,0.0,blended);
	//displayImage("blended",blended);
	//cv::imwrite("blended.bmp",blended);


	//Warp warp;
	//////
	//cv::Mat rotatedImage;
	//for(int i=0;i<361;i+=10){
	//   //warp.TestTransformation(image1,i,0,0);
	//	cv::Point topLeft, bottomRight;
	//	cv::Mat homography(3,3,CV_64F);
	//	warp.GetCustomHomography(i,0,0,homography);
	//	warp.RotateImage(image1,homography,rotatedImage,topLeft,bottomRight);
	//	char buffer[100];
	//	sprintf(buffer,"output/Rotation/Rotation_%d.png",i);
	//	cv::imwrite(buffer,rotatedImage);
	//	cv::imshow("rotation",rotatedImage);
	//	cv::waitKey(0);

	//}
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
	//%cv::Mat tmpImage;
	/*detector.drawOnImage(tmpImage,pts);
	cv::imshow("harris Points",tmpImage);
	cv::waitKey(0);*/	
	

	//%Corners corner;
	//%std::vector<cv::KeyPoint> keyPoints1,keyPoints2;	

	//corner.GetSurfFeatures(image1,keyPoints1);
	//corner.GetSurfFeatures(image2,keyPoints2);
	//%MyTimer timer;
	//%timer.start("Points detection for image1 started...");
	//corner.GetDynamicAdaptedFeatures_SURF(image1,1500,4700,keyPoints1,130);
	//%corner.GetSurfFeatures(image1,keyPoints1);
	//%timer.check();
	//%timer.start("Points detection for image1 started...");
	//corner.GetDynamicAdaptedFeatures_SURF(image2,1500,4700,keyPoints2,130);
	//%corner.GetSurfFeatures(image2,keyPoints2);
	//%timer.check();


	//SD
	/*Arithmatic arithmatic;
	arithmatic.CalculateSD(image1);*/
	//corner.getImageInformation(image1);


	//>>>>>>>>>>>>> DISPLAY
	//%cv::drawKeypoints(image1,keyPoints1,tmpImage);
	////cv::imshow("keypoints1",tmpImage);
	////cv::waitKey(0);
	//displayImage("KeyPoints1",tmpImage);
	//%cv::imwrite("output/o_Image1(keyPoints).bmp",tmpImage);

	//%cv::drawKeypoints(image2,keyPoints2,tmpImage);
	//displayImage("keypoints2",tmpImage);
	////cv::waitKey(0);
	//%cv::imwrite("output/o_Image2(keyPoints).bmp",tmpImage);
	//
	//sprintf(szBuffer, "Key Points1=%i\nKey Point2=%d", keyPoints1.size(),keyPoints2.size());
	//MessageBoxA(NULL,szBuffer,"Key Points Result",MB_OK);	
	//ENDISPLAY   <<<<<<<<<<<<<<<


	//%Matching matching;
	//%std::vector<std::vector<cv::DMatch>> matches1,matches2;
	//%timer.restart("Matching...");
	//%matching.GetMatchesSurf(image1,image2,keyPoints1,keyPoints2,matches1,matches2);	
	//%timer.check();
	
	// >>>>>>>>>>>>>>> DISPLAY
	/*sprintf(szBuffer,"image1->image2 matches=%d\nimage2->image1 matches=%d",
		matches1.size(),matches2.size());
	MessageBoxA(NULL,szBuffer,"Matching Result",MB_OK);*/
	// ENDISPLAY <<<<<<<<<<<<<<<<<

	//%timer.restart("Ratio Test1...");
	//%int removed1=matching.RatioTest(matches1,0.8);	
	//%timer.check();
	//%timer.restart("Ratio Test2...");
	//%int removed2=matching.RatioTest(matches2,0.8);	
	//%timer.check();
	
	//>>>>>>>>>>>>>>>>>>DISPLAY
	/*sprintf(szBuffer,"Removed Points\n image1=%d\n image2=%d",
		removed1,removed2);
	MessageBoxA(NULL,szBuffer,"Ratio Test",MB_OK);*/
	//ENDISPLAY  <<<<<<<<<<<<<<<

	//%std::vector<cv::DMatch> symmetryMatches;
	//%timer.restart("Symmetry Matches....");
	//%matching.SymmetryTest(matches1,matches2,symmetryMatches);
	//%timer.check();

	//>>>>>>>>>>>>>>>>DISPLAY
	//%matching.DrawMatches(image1,keyPoints1,image2,keyPoints2,symmetryMatches,tmpImage);
	//displayImage("Symmetry Matches",tmpImage);
	////cv::waitKey(0);
	//%cv::imwrite("output/o_SymmetryMatches.bmp",tmpImage);

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

	//%cv::Mat imageMatches;	
	//%std::vector<uchar> inliers;
	//%cv::Mat homography;
	//%timer.restart("Homography...");
	//%homography=matching.GetHomography(symmetryMatches,keyPoints1,keyPoints2,inliers);	
	//%timer.check();

 
	
	

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

	//%std::vector<cv::Point2f> points1,points2;
	//%matching.GetFloatPoints(keyPoints1,keyPoints2,symmetryMatches,points1,points2);
	//%matching.DrawInliers(points1,inliers,image1,tmpImage);
	//Utility::DisplayImage("inlier1",tmpImage);
	//cv::waitKey(0);
	//%cv::imwrite("output/o_Image1(inliers).bmp",tmpImage);

	//%matching.DrawInliers(points2,inliers,image2,tmpImage);
	//Utility::DisplayImage("inlier2",tmpImage);
	//cv::waitKey(0);
	//%cv::imwrite("output/o_Image2(inliers).bmp",tmpImage);

	//*********	

	//We have homography matrix, now the final task 
	//is to transform image1 on image 2 and stitch together

	

	//cv::Mat destination;
	/*cv::warpPerspective(image1,destination,homography,cv::Size(image1.cols*2,image1.rows*2),CV_WARP_FILL_OUTLIERS);
	displayImage("warp",destination);
	cv::imwrite("o_Warp.bmp",destination);*/

	/*cv::Mat output;	
	Warp warp;
	
	warp.RotateImage(image1,output,homography);*/

	//Stitching Process starts
	Stitching stitching(image1,image2);
	stitching.Stitch();
	std::getchar();	
}