#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include<fstream>

/***
* @class Utilitz
* @description This is utilitz class which includes the common 
*  operations for image processing
* @author Krishna(krishna444@gmail.com	
* @date 2012-05-08 11:08:45
***/
class Utility{
public:
	//Displays Image 
	void DisplayImage(char* title, cv:: Mat& image);
	//Draws Rectangle, the default color is white
	void DrawRectangle(cv::Rect rect,cv::Mat& outputImage,cv::Scalar color=cv::Scalar(255,255,255));
	//Fills the rectangle with color
	void FillRectangle(cv::Rect rect,cv::Mat& outputImage, cv::Scalar color=cv::Scalar(128,128,128));
	//Records objects in the file
	void WriteHomography(std::string title,cv::Mat homography);
};

