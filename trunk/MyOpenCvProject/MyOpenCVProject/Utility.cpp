#include "Utility.h"

void Utility::DisplayImage(char* title, cv:: Mat& image){
	cv::Mat tmpImage;	
	cv::resize(image,tmpImage,image.size());
	cv::imshow(title,tmpImage);
	cv::waitKey(0);
}


void Utility::DrawRectangle(cv::Rect rect,cv::Mat& outputImage,cv::Scalar color){	
	cv::line(outputImage,cv::Point(rect.x,rect.y),cv::Point(rect.x+rect.width,rect.y),cv::Scalar(255,255,255));
	cv::line(outputImage,cv::Point(rect.x+rect.width,rect.y),cv::Point(rect.x+rect.width,rect.y+rect.height),cv::Scalar(255,255,255));
	cv::line(outputImage,cv::Point(rect.x+rect.width,rect.y+rect.height),cv::Point(rect.x,rect.y+rect.height),cv::Scalar(255,255,255));
	cv::line(outputImage,cv::Point(rect.x,rect.y+rect.height),cv::Point(rect.x,rect.y),cv::Scalar(255,255,255));	
}

void Utility::FillRectangle(cv::Rect rect,cv::Mat& outputImage, cv::Scalar color){
	cv::Point points[]={cv::Point(rect.x,rect.y),cv::Point(rect.x+rect.width,rect.y),cv::Point(rect.x+rect.width,rect.y+rect.height),cv::Point(rect.x,rect.y+rect.height)};
	cv::fillConvexPoly(outputImage,points,4,color);
}
void Utility::WriteHomography(std::string title,cv::Mat homography){
	std::fstream file;
	file.open("homography.txt",std::ios::app);
	file<<"\n"<<title<<"=\n"<<homography;
}

void Utility::WriteContent(std::string content){
	std::fstream file;
	file.open("contents.txt",std::ios::app);
	file<<content;
}