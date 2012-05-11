#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

/***
* @class Utilitz
* @description This is utilitz class which includes the common 
*  operations for image processing
* @author Krishna(krishna444@gmail.com	
* @date 2012-05-08 11:08:45
***/
class Utility{
public:
	void DisplayImage(char* title, cv:: Mat& image);
	void DrawRectangle(cv::Rect rect, cv::Mat& image,cv::Mat& outputImage);
};

