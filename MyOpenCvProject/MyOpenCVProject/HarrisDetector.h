/**
* @class HarrisDetector
* @brief Harris Detector Class. Implements all the steps of Harris detection. 
  @author Krishna Paudel (krishna444@gmai.com)
**/
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

class HarrisDetector{
private: 
	//image of corner strength value
	cv::Mat cornerStrength;
	//image of thresholded corners
	cv::Mat cornerTh;
	//image of localmax
	cv::Mat localMax;
	//size of neighboorhood for derivative smoothing
	int neighbourhood;
	//aperture for gradient computation
	int aperture;
	//Harris Parameter
	double k;
	// maximum strength for threshold computation
	double maxStrength;
	//calculate threshold
	double threshold;
	//size of neighborhood for non-maximum suppression
	int nonMaxSize;
	//kerner for non-max suppression
	cv::Mat kernel;
	// Get the corner map from the computed Harris values
	cv::Mat getCornerMap(double qualityLevel);
	
	//Get the feature points from the computed corner map
	void getCorners(std::vector<cv::Point>&,const cv::Mat& cornerMap);

public:
	HarrisDetector();
	// Compute Harris corners
	void detect(const cv::Mat& image);
	//get the feature points from the computed corner map
	void getCorners(std::vector<cv::Point>&,double qualityLevel);
	//Draw circles at the feature point locations on an image
	void drawOnImage(cv::Mat& image,
		const std::vector<cv::Point>& point,
		cv::Scalar color=cv::Scalar(255,255,255),
		int radius=3,int thickness=2);
};
