/* @class: Blender
   @author: Krishna Paudel
   @date: 2012-05-24 15:29:30
   @description: This is Aplha blender.This blends the images in two directions. 
   @TODO: More study regarding alpha blending to get more accurate results.
   */
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Stitching.h"

class AlphaBlender{
private:
	//Blends along X-direction
	void performBlendX(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage);
	//Blends along Y-direction
	void performBlendY(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage);
	//Levels pixel
	void levelPixels(cv::Mat& image1,cv::Mat& image2);

public:
	//Blends two images along horizontal and vertical
	cv::Mat blend(const cv::Mat& image1,const cv::Mat& image2,
	Boundry& left,Boundry& top,Boundry& right,Boundry& bottom,
	cv::Mat& outputImage);
};