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
#include <Windows.h>
#include <string.h>

int main(void)  
{
	//For message display
	char szBuffer[100];	

	
	/*cv::Mat image1=cv::imread("Splitted_1.png",0);
	cv::Mat image2=cv::imread("Splitted_2.png",0);*/
	/*cv::Mat image1=cv::imread("1.jpg",0);
	cv::Mat image2=cv::imread("2.jpg",0);*/

	cv::Mat image1=cv::imread("knee_1.jpg",0);
	cv::Mat image2=cv::imread("knee_3_mr.jpg",0);


	if(!image1.data ||!image2.data){
		printf("Error: Image Not Found!");
		std::getchar();		
	}


	HarrisDetector detector;
	detector.detect(image1);
	std::vector<cv::Point> pts;
	detector.getCorners(pts,0.00001);
	cv::Mat tmpImage;
	/*detector.drawOnImage(tmpImage,pts);
	cv::imshow("harris Points",tmpImage);
	cv::waitKey(0);*/	
	

	Corners corner;
	std::vector<cv::KeyPoint> keyPoints1,keyPoints2;	

	corner.GetSurfFeatures(image1,keyPoints1);
	corner.GetSurfFeatures(image2,keyPoints2);

	//>>>>>>>>>>>>> DISPLAY
	sprintf(szBuffer, "Key Points1=%i\nKey Point2=%d", keyPoints1.size(),keyPoints2.size());
	MessageBoxA(NULL,szBuffer,"Key Points Result",MB_OK);	

	cv::drawKeypoints(image1,keyPoints1,tmpImage);
	cv::imshow("keypoints1",tmpImage);
	cv::waitKey(0);

	cv::drawKeypoints(image2,keyPoints2,tmpImage);
	cv::imshow("keypoints2",tmpImage);
	cv::waitKey(0);	
	
	//ENDISPLAY   <<<<<<<<<<<<<<<


	Matching matching;
	std::vector<std::vector<cv::DMatch>> matches1,matches2;
	matching.GetMatchesSurf(image1,image2,keyPoints1,keyPoints2,matches1,matches2);	
	
	// >>>>>>>>>>>>>>> DISPLAY
	sprintf(szBuffer,"image1->image2 matches=%d\nimage2->image1 matches=%d",
		matches1.size(),matches2.size());
	MessageBoxA(NULL,szBuffer,"Matching Result",MB_OK);
	// ENDISPLAY <<<<<<<<<<<<<<<<<


	int removed1=matching.RatioTest(matches1,0.8);	
	int removed2=matching.RatioTest(matches2,0.8);	

	//>>>>>>>>>>>>>>>>>>DISPLAY
	sprintf(szBuffer,"Removed Points\n image1=%d\n image2=%d",
		removed1,removed2);
	MessageBoxA(NULL,szBuffer,"Ratio Test",MB_OK);
	//ENDISPLAY  <<<<<<<<<<<<<<<

	std::vector<cv::DMatch> symmetryMatches;
	matching.SymmetryTest(matches1,matches2,symmetryMatches);

	//>>>>>>>>>>>>>>>>DISPLAY
	sprintf(szBuffer,"Selected Matches=%d",
		symmetryMatches.size());
	MessageBoxA(NULL,szBuffer,"Symmetry Test",MB_OK);

	matching.DrawMatches(image1,keyPoints1,image2,keyPoints2,symmetryMatches,tmpImage);
	cv::imshow("Symmetry Matches",tmpImage);
	cv::waitKey(0);
	//ENDISPLAY  <<<<<<<<<<<<<<<


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
	homography=matching.GetHomography(symmetryMatches,keyPoints1,keyPoints2,inliers);


 
	
	

	//**********DISPLAY
	sprintf(szBuffer,"Inliers Count=%d",
		inliers.size());
	MessageBoxA(NULL,szBuffer,"Homography Result",MB_OK);

	std::vector<cv::Point2f> points1,points2;
	matching.GetFloatPoints(keyPoints1,keyPoints2,symmetryMatches,points1,points2);
	matching.DrawInliers(points1,inliers,image1,tmpImage);
	cv::imshow("inlier1",tmpImage);
	cv::waitKey(0);

	matching.DrawInliers(points2,inliers,image2,tmpImage);
	cv::imshow("inlier2",tmpImage);
	cv::waitKey(0);
	//*********

	cv::Mat result;
	cv::warpPerspective(image1,result,homography,cv::Size(2*image1.cols,image1.rows));
	//cv::imshow("warp ",result);
	cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
	cv::imshow("half ",half);
	cv::waitKey(0);
}
