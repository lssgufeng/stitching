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

int main(void)  
{
	HarrisDetector detector;
	/*cv::Mat image1=cv::imread("Splitted_1.png",0);
	cv::Mat image2=cv::imread("Splitted_2.png",0);
*/
	cv::Mat image1=cv::imread("1.jpg",0);
	cv::Mat image2=cv::imread("2.jpg",0);

	/*cv::Mat image1=cv::imread("knee_1.jpg",0);
	cv::Mat image2=cv::imread("knee_3_mr.jpg",0);*/
	if(!image1.data ||!image2.data){
		printf("Error: Image Not Found!");
		std::getchar();		
	}
	detector.detect(image1);
	std::vector<cv::Point> pts;
	detector.getCorners(pts,0.00001);
	cv::Mat tmpImage=image1.clone();

	/*detector.drawOnImage(tmpImage,pts);
	cv::imshow("harris Points",tmpImage);
	cv::waitKey(0);*/

	Corners corner;
	std::vector<cv::KeyPoint> keyPoints1,keyPoints2;

	//corner.GetGoodFeaturesToTrack(image1,keyPoints1);
	//corner.GetGoodFeaturesToTrack(image2,keyPoints2);

		
	/*corner.GetFastFeatures(image1,keyPoints1);
	corner.GetFastFeatures(image2,keyPoints2);*/

	/*corner.GetSiftFeatures(image1,keyPoints1);
	corner.GetSiftFeatures(image2,keyPoints2);*/

	corner.GetSurfFeatures(image1,keyPoints1);
	corner.GetSurfFeatures(image2,keyPoints2);

	printf("KeyPoints1=%d",keyPoints1.size());
	printf("KeyPoints2=%d",keyPoints2.size());

	cv::drawKeypoints(image1,keyPoints1,tmpImage);
	cv::imshow("keypoints1",tmpImage);
	cv::waitKey(0);


	Matching matching;
	std::vector<std::vector<cv::DMatch>> matches1,matches2;
	matching.GetMatchesSurf(image1,image2,keyPoints1,keyPoints2,matches1,matches2);	

	
	int removed=matching.RatioTest(matches1,0.8);
	printf("%d points removed",removed);
	removed=matching.RatioTest(matches2,0.8);
	printf("%d points removed",removed);

	std::vector<cv::DMatch> symmetryMatches;
	matching.SymmetryTest(matches1,matches2,symmetryMatches);
	printf("Symmetric Test Result=%d Selected.",symmetryMatches.size());

	/*
	std::vector<cv::DMatch> ransacMatches;
	matching.RansacTest(symmetryMatches,keyPoints1,keyPoints2,3.0,0.9,ransacMatches);
	printf("After RANSAC=%d points.",ransacMatches.size());
	*/

	/* Get Top 14 matches */
	//std::nth_element(matches1.begin(),matches1.begin()+24,matches1.end());
	//matches1.erase(matches1.begin()+24,matches1.end());

	cv::Mat imageMatches;

	/*
	cv::drawMatches(image1,keyPoints1,
		image2,keyPoints2,matches1,imageMatches);
	cv::imshow("Matches RatioTest",imageMatches);
	cv::waitKey(0);


	cv::drawMatches(image1,keyPoints1,
		image2,keyPoints2,symmetryMatches,imageMatches);
	cv::imshow("Matches Symmetry",imageMatches);
	cv::waitKey(0);

	
	
	cv::drawMatches(image1,keyPoints1,
		image2,keyPoints2,ransacMatches,imageMatches);
	cv::imshow("Matches RANSAC",imageMatches);
	cv::waitKey(0);
	*/
	std::vector<uchar> inliers;
	cv::Mat homography;
	homography=matching.GetHomography(symmetryMatches,keyPoints1,keyPoints2,inliers);
	printf("Homography\n");
	for(int i=0;i<homography.rows;i++){
		for(int j=0;j<homography.cols;j++){
			printf("%f ~~~~",*homography.data+i+j);
		}
		printf("\n");
	}

 
	cv::Mat outputImage; 
	std::vector<cv::Point2f> points1,points2;
	matching.GetFloatPoints(keyPoints1,keyPoints2,symmetryMatches,points1,points2);
	matching.DrawInliers(points1,inliers,image1,outputImage);
	cv::imshow("inlier1",outputImage);
	cv::waitKey(0);

	matching.DrawInliers(points2,inliers,image2,outputImage);
	cv::imshow("inlier2",outputImage);
	cv::waitKey(0);

	cv::Mat result;
	cv::warpPerspective(image1,result,homography,cv::Size(2*image1.cols,image1.rows));
	//cv::imshow("warp ",result);
	cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
	cv::imshow("half ",half);
	cv::waitKey(0);

	image2.copyTo(half);
	cv::imshow("copy ",half);
	cv::waitKey(0);

}
