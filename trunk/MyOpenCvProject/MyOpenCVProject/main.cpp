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
	cv::Mat image1=cv::imread("Splitted_1.png",0);
	cv::Mat image2=cv::imread("Splitted_Rotated_2.png",0);
	/*cv::Mat image1=cv::imread("knee_1.bmp",0);
	cv::Mat image2=image1.clone();*/
	if(!image1.data ||!image2.data){
		printf("Error: Image Not Found!");
		std::getchar();		
	}
	detector.detect(image1);
	std::vector<cv::Point> pts;
	detector.getCorners(pts,0.00001);
	cv::Mat tmpImage=image1.clone();

	detector.drawOnImage(tmpImage,pts);
	cv::imshow("harris Points",tmpImage);
	cv::waitKey(0);

	Corners corner;
	std::vector<cv::KeyPoint> keyPoints1,keyPoints2;

	//corner.GetGoodFeaturesToTrack(image1,keyPoints1);
	//corner.GetGoodFeaturesToTrack(image2,keyPoints2);

		
	corner.GetFastFeatures(image1,keyPoints1);
	corner.GetFastFeatures(image2,keyPoints2);

	/*corner.GetSiftFeatures(image1,keyPoints1);
	corner.GetSiftFeatures(image2,keyPoints2);

	corner.GetSurfFeatures(image1,keyPoints1);
	corner.GetSurfFeatures(image2,keyPoints2);*/

	printf("KeyPoints1=%d",keyPoints1.size());
	printf("KeyPoints2=%d",keyPoints2.size());

	cv::drawKeypoints(image1,keyPoints1,tmpImage);
	cv::imshow("keypoints1",tmpImage);
	cv::waitKey(0);


	Matching matching;
	std::vector<cv::DMatch> matches1,matches2;
	matching.GetMatchesSurf(image1,image2,keyPoints1,keyPoints2,matches1,matches2);	


	/* Get Top 14 matches */
	std::nth_element(matches1.begin(),matches1.begin()+24,matches1.end());
	matches1.erase(matches1.begin()+24,matches1.end());

	cv::Mat imageMatches;
	cv::drawMatches(image1,keyPoints1,
		image2,keyPoints2,matches1,imageMatches);
	cv::imshow("Matches",imageMatches);
	cv::waitKey(0);
}
