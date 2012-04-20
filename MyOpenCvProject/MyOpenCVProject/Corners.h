/**
* @class Corners
* @description Detects the corners. The methods here are already implemented by the library
* @author Krishna
**/
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"

class Corners{
private: 
	//Maximum number of corners to be detected
	int maxCorners;
	//Quality level
	double qualityLevel;
	//Minimum distance between the points
	int minDistance;
	//Threshold for fast feature detection
	int threshold;
public:
	Corners();
	//Get the good features 
	void GetGoodFeaturesToTrack(const cv::Mat& image,std::vector<cv::KeyPoint> keyPoints);
	//Get the fast features. It uses different algorithm 
	void GetFastFeatures(const cv::Mat& image,std::vector<cv::KeyPoint> keyPoints);
	//Draw the detected keypoints
	void DrawKeyPoints(const cv::Mat& originalImage,std::vector<cv::KeyPoint> keyPoints,
		cv::Mat& outputImage);
};
