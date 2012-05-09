/***
* @class Stitching
* @description This performs the final operations 
	of stitching. After we get the rotated image of 
	the floating image, the image is stitched with 
	the base image.
	Initial assumption is we have the rotated float image,the base image and homography
	###########
	TODO: implement everything here i.e. it takes two images and gives the final result. 
	###########
* @author Krishna Paudel(krishna444@gmail.com)
* @date 2012-05-08 13:58:34
***/

#include "opencv2\core\core.hpp"

class Stitching{
private:
	//Floating image
	cv::Mat floatingImage;
	//Base Image
	cv::Mat baseImage;
	//Homography
	cv::Mat homography;	
public: 
	//Constructor
	//@floatingImage Floating Image
	//@baseImage Base Image, which is fixed.
	//@homography Transformation matrix 
	Stitching(cv::Mat floatingImage, cv::Mat baseImage, cv::Mat homography);
	//destructor
	~Stitching();
	//it performs the main task of stitching
	void Stitch();
};