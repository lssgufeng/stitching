/***
@class LaplacianBlender
@description It performs the blending using Laplacian Pyramids
             Builds the pyramid of laplacian images and then 
			 it regenerates the blended image from low resolution
			 to high resolution.
@author Krishna Paudel(krishna444@gmail.com)
@date 2012-05-31 11:11:12
**/
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Stitching.h"
class LaplacianBlender{
/*Private Variables*/
private:
	//Two images to blend
	cv::Mat_<cv::Vec3f> floatImage, baseImage;
	//Laplacian Pyramids
	cv::Vector<cv::Mat_<cv::Vec3f>> floatLapPyr, baseLapPyr, resultLapPyrX,resultLapPyrY;
	
	//Smallest Images
	cv::Mat floatSmallestLevel, baseSmallestLevel, resultSmallestLevelX,resultSmallestLevelY;
	//Mask Gaussian Pyramid
	cv::Vector<cv::Mat_<cv::Vec3f>> maskGaussianPyramidX,maskGaussianPyramidY;
	//Blend Mask
	cv::Mat_<float> blendMaskX,blendMaskY;
	//Levels
	int levels;
/* Private Functions*/
private:
	//Generate the Laplacian pyramid of the image
	//@image image to generate Laplacian Pyramid
	//@lapPyr Vector to store laplacian pyramid
	//@smallestLevel the smallest image at the top of the pyramid
	void generateLaplacianPyramid(const cv::Mat& image,cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr,
		cv::Mat& smallestLevel);
	//Generate the Gaussian Pyramids
	//one for x-direction and one for y-direction
	void generateGaussianPyramids();
	//This creates the gaussian pyramid
	void LaplacianBlender::generateGaussianPyramid(cv::Mat& blendMask,
		cv::Vector<cv::Mat_<cv::Vec3f>>& maskGaussianPyramid);	
	//Blend Pyramids in X-direction
	//First pyramid is left and second pyramid is for right
	void blendLapPyrsX(cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr1,cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr2,
		cv::Mat& smallestLevel1,cv::Mat& smallestLevel2);
	//Blend Pyramids in Y-direction
	//First pyramid is effective for top part and Second pyramid is effective for bottom part
	void blendLapPyrsY(cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr1,cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr2,
		cv::Mat& smallestLevel1,cv::Mat& smallestLevel2);
	//Generate Blended Image in X-direction
	cv::Mat reconstructImageX();
	//Generate Blended Image in Y-direction
	cv::Mat reconstructImageY();


public:
	//Constructor
	LaplacianBlender(const cv::Mat& floatImage,const cv::Mat& baseImage);
	//Blends two images along horizontal and vertical direction
	void blend(Boundry& left,Boundry& top,Boundry& right,Boundry& bottom,
		cv::Mat& outputImage);
};