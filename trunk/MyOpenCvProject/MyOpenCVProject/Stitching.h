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
//Defines the boundry point
//@Value is the co-ordinate value
//@Index is image index
struct Boundry{
	int Value;
	int Index;
};

class Stitching{
private:
	char szBuffer[100];
	//Floating image
	cv::Mat floatingImage;
	//Base Image
	cv::Mat baseImage;
	//Rotated Image
	cv::Mat rotatedImage;
	//Homography
	cv::Mat homography;	


private:
	//It blends the two images. Note: the size of the images should be same
	//@image1 First image(Floating image)
	//@image2 Second image(Base image)
	//@top top image	
	//@left left image
	void blend(cv::Mat image1,cv::Mat image2,cv::Mat outputImage,
		Boundry& left,Boundry& top,Boundry& right,Boundry& bottom);
	//blend two images starting alpha=1 to aplha=0
	//Note: order is important here.
	void performBlendX(cv::Mat image1,cv::Mat image2,cv::Mat& outputImage);
	void performBlendY(cv::Mat image1,cv::Mat image2,cv::Mat& outputImage);

	

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

