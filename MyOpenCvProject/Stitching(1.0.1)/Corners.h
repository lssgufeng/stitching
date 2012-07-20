/**
* @class Corners
* @description Detects the corners. The methods here are already implemented by the library
* @author Krishna Paudel(krishna444@gmail.com)
**/
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include <process.h>
#define NOMINMAX
#include<windows.h>

struct threadDataSurf{
	cv::Mat image;
	std::vector<cv::KeyPoint>& keyPoints;
};
void surfFeatures(void* threadArg);


class Corners{
private: 
	//if any private variables..include here
	cv::Ptr<cv::FeatureDetector> detector;
	cv::Ptr<cv::DynamicAdaptedFeatureDetector> dymanicDetector;
	
public:
	//Measure the image variations
	double Corners::getImageInformation(cv::Mat&);
	//Adjuster for getting dynamic adapted features
	enum Adjuster {FAST,SIFT,SURF};
	Corners();
	//Get the good features 
	void GetGoodFeaturesToTrack(const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints);
	//Get the fast features. It uses different algorithm 
	void GetFastFeatures(const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints);
	//SURF features
	void GetSurfFeatures(const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints);
	void Corners::GetSurfFeaturesThread(const cv::Mat& image1, 
	std::vector<cv::KeyPoint>& keyPoints1, const cv::Mat& image2,std::vector<cv::KeyPoint>& keyPoints2);
	//SIFT features
	void GetSiftFeatures(const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoitns);
	//To obtain the corner points within a defined range
	void GetDynamicAdaptedFeatures_SURF(cv::Mat& image,int minFeatures, 
		int maxFeatures,std::vector<cv::KeyPoint>& keyPoints,int iterations);
	//To obtain the corner points within a defined range
	void GetDynamicAdaptedFeatures_FAST(cv::Mat& image,int minFeatures, 
		int maxFeatures,std::vector<cv::KeyPoint>& keyPoints,int iterations);
	//To obtain the corner points within a defined range
	void GetDynamicAdaptedFeatures_STAR(cv::Mat& image,int minFeatures, 
		int maxFeatures,std::vector<cv::KeyPoint>& keyPoints,int iterations);
	//Draw the detected keypoints	
	void DrawKeyPoints(const cv::Mat& originalImage,std::vector<cv::KeyPoint>& keyPoints,
		cv::Mat& outputImage);
	

};
