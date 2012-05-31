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
class LaplacianBlender{
/*Private Variables*/
private:
	//Laplacian Pyramids
	cv::Vector<cv::Mat_<cv::Vec3f>> image1LapPyr, image2LapPyr, resultLapPyrX,resultLapPyrY;
	
	//Smallest Images
	cv::Mat smallestLevel1, smallestLevel2, resultSmallestLevelX,resultSmallestLevelY;
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
	//Blend Pyramids in X-direction
	void blendLapPyrsX();
	//Blend Pyramids in Y-direction
	void blendLapPyrsY();
	//Generate Blended Image in X-direction
	void reconstructImageX();
	//Generate Blended Image in Y-direction
	void reconstructImageY();


public:
	LaplacianBlender();
	//Blends two images along horizontal and vertical direction
	void blend(const cv::Mat& image1,const cv::Mat& image2,
		Boundry& left,Boundry& top,Boundry& right,Boundry& bottom,
		cv::Mat& outputImage);


}