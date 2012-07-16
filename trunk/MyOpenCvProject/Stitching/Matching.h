/***
* @ class Matching
* @ description matching process is carried out after we detect the key points. 
* @ author Krishna (krishna444@gmail.com)
**/
#include "opencv2\calib3d\calib3d.hpp"
#include "opencv2\contrib\contrib.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include<process.h>
#define NOMINMAX
#include<windows.h>

struct threadData{
	cv::Mat descriptors1;
	cv::Mat descriptors2;
	std::vector<std::vector<cv::DMatch>>& matches;
};
void knnMatch(void* threadArg);

class Matching {
private:
	//private members
	cv::Ptr<cv::DescriptorExtractor> extractor;
	cv::Mat descriptors1,descriptors2;
	void performMatching(cv::Mat descriptors1,cv::Mat descriptors2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);
	
public:
	//Get the matches using the Brief Descriptors
	void GetMatchesBrief(cv::Mat& image1,cv::Mat& image2,
		std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);	
	//Get the matches using Surf Descriptors
	void GetMatchesSurf(cv::Mat& image1,cv::Mat& image2,
		std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);
	void GetMatchesSurfThread(cv::Mat& image1,cv::Mat& image2,
		std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);
	//Get the matches using Sift Descriptors
	void GetMatchesSift(cv::Mat& image1,cv::Mat& image2,
		std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);

	//Performs the ratio test of the matches found
	int RatioTest(std::vector<std::vector<cv::DMatch>>& matches,double threshold);
	//Perform the symmetry test. Returns the matches selected
	void SymmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
		const std::vector<std::vector<cv::DMatch>>& matches2,
		std::vector<cv::DMatch>& symMatches);
	//Perform RANSAC Test to get the best matched points. It returns fundamental matrix
	cv::Mat RansacTest(const std::vector<cv::DMatch>& goodMatches,
		const std::vector<cv::KeyPoint>& keyPoints1,
		const std::vector<cv::KeyPoint>& keyPoints2,
		double distance, double confidence,
		std::vector<cv::DMatch>& resultMatches);
	//Find the homography matrix 
	cv::Mat GetHomography(const std::vector<cv::DMatch>& goodMatches, 
		std::vector<cv::KeyPoint>& keyPoints1,
		std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<uchar>& inliers);
	void DrawMatches(cv::Mat image1,std::vector<cv::KeyPoint> keyPoints1,
		cv::Mat image2,std::vector<cv::KeyPoint> keyPoints2,
		std::vector<cv::DMatch> matches,cv::Mat& outputImage);

	//Draw the inliers in the image
	void DrawInliers(std::vector<cv::Point2f> points,std::vector<uchar>& inliers, cv::Mat image,cv::Mat& outImage);
	//get the float point conversion of key point which is need for conversion
	void GetFloatPoints(const std::vector<cv::KeyPoint>& keyPoints1,const std::vector<cv::KeyPoint>& keyPoints,const std::vector<cv::DMatch>&matches,
		std::vector<cv::Point2f>& points1,std::vector<cv::Point2f>& points2);
};