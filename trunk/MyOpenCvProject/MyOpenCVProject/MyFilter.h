#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"

/***
* @class MyFilter
* @description This class is for filter. 
* @author Krishna Paudel(krishna444@gmail.com)
* @date 2012-04-27 14:04
***/
class MyFilter{
private:

public:
	//Sobel filter. Purpose: to get the variations in the image
	void Filter_Sobel(cv::Mat& image, cv::Mat& outputImage);
	//Laplace filter. Purpose: to get the variations in the image
	void Filter_Laplacian(cv::Mat& image, cv::Mat& outputImage);
};


void MyFilter::Filter_Sobel(cv::Mat& image,cv::Mat& outputImage){
	cv::Mat sobelX,sobelY;
	cv::Sobel(image,sobelX,CV_16S,1,0);
	cv::Sobel(image,sobelY,CV_16S,0,1);
	outputImage=abs(sobelX)+abs(sobelY);
}
