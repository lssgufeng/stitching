/**
* @class Corners
* @description Detects the corners. The methods here are already implemented by the library
* @author Krishna Paudel(krishna444@gmail.com)
**/
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"

class Corners{
private: 
	//if any private variables..include here
	cv::Ptr<cv::FeatureDetector> detector;
public:
	Corners();
	//Get the good features 
	void GetGoodFeaturesToTrack(const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints);
	//Get the fast features. It uses different algorithm 
	void GetFastFeatures(const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints);
	//SURF features
	void GetSurfFeatures(const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoints);
	//SIFT features
	void GetSiftFeatures(const cv::Mat& image,std::vector<cv::KeyPoint>& keyPoitns);
	//Draw the detected keypoints
	void DrawKeyPoints(const cv::Mat& originalImage,std::vector<cv::KeyPoint>& keyPoints,
		cv::Mat& outputImage);

};
