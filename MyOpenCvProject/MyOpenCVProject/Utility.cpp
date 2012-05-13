#include "Utility.h"

void Utility::DisplayImage(char* title, cv:: Mat& image){
	cv::Mat tmpImage;	
	cv::resize(image,tmpImage,image.size());
	cv::imshow(title,tmpImage);
	cv::waitKey(0);
}


void Utility::DrawRectangle(cv::Rect rect, cv::Mat& image,cv::Mat& outputImage){
	outputImage=image.clone();
	cv::line(outputImage,cv::Point(rect.x,rect.y),cv::Point(rect.x+rect.width,rect.y),cv::Scalar(255,255,255));
	cv::line(outputImage,cv::Point(rect.x+rect.width,rect.y),cv::Point(rect.x+rect.width,rect.y+rect.height),cv::Scalar(255,255,255));
	cv::line(outputImage,cv::Point(rect.x+rect.width,rect.y+rect.height),cv::Point(rect.x,rect.y+rect.height),cv::Scalar(255,255,255));
	cv::line(outputImage,cv::Point(rect.x,rect.y+rect.height),cv::Point(rect.x,rect.y),cv::Scalar(255,255,255));
	cv::imshow("OUTPUT",outputImage);

}