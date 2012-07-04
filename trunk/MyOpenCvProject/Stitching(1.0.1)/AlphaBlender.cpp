#include "AlphaBlender.h"

cv::Mat AlphaBlender::blend(const cv::Mat& firstImage,const cv::Mat& secondImage,
	const Boundry& left,const Boundry& top,const Boundry& right,const Boundry& bottom,
	cv::Mat& outputImage){
		cv::Mat image1=firstImage.clone();
		cv::Mat image2=secondImage.clone();
		this->levelPixels(image1,image2);
		cv::Mat tmpImageX(image1.rows,image2.cols,CV_16U), tmpImageY(image1.rows,image1.cols,CV_16U);
        //X-direction
		if(left.Index==0){
			if(right.Index==0){
				performBlendX(image1,image1,tmpImageX);
			}else{
				performBlendX(image1,image2,tmpImageX);
			}
		}else{
			if(right.Index==0){
				performBlendX(image2,image1,tmpImageX);
			}else{
				performBlendX(image2,image2,tmpImageX);
			}
		}

		//Y-direction
		if(top.Index==0){
			if(bottom.Index==0){
				performBlendY(image1,image1,tmpImageY);
			}else{
				performBlendY(image1,image2,tmpImageY);
			}
		}else{
			if(bottom.Index==0){
				performBlendY(image2,image1,tmpImageY);
			}else{
				performBlendY(image2,image2,tmpImageY);
			}
		}

		cv::addWeighted(tmpImageX,0.5,tmpImageY,0.5,0,outputImage);
		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				double weightX=-1;
				if(i==0 && j==0)
					weightX=1;
				else if(j<=image1.cols/2 && i<=image1.rows/2)
					weightX=1.0-(double)j/(i+j);
				else if(j<=image1.cols/2 && i>image1.rows/2)
					weightX=1.0-(double)j/(j+(image1.rows-i));
				else if(j>image1.cols/2 && i<=image1.rows/2)
					weightX=1.0-(image1.cols-(double)j)/((image1.cols-j)+i);
				else 
					weightX=1.0-(image1.cols-(double)j)/((image1.cols-j)+(image1.rows-i));

				outputImage.at<ushort>(i,j)=/*255*weightX;*/tmpImageX.at<ushort>(i,j)*weightX+tmpImageY.at<ushort>(i,j)*(1-weightX);
			}
		}
		return outputImage;
}
cv::Mat AlphaBlender::blend(const cv::Mat& firstImage,
	const cv::Mat& secondImage,cv::Mat& outputImage){
		cv::Mat image1=firstImage.clone();
		cv::Mat image2=secondImage.clone();
		this->levelPixels(image1,image2);
		cv::Mat tmpImageX(image1.rows,image2.cols,CV_16U), tmpImageY(image1.rows,image1.cols,CV_16U);
		performBlendX(image1,image2,tmpImageX);
		performBlendY(image1,image2,tmpImageY);
		for(int i=0;i<image1.rows;i++){
			for(int j=0;j<image1.cols;j++){
				double weightX=-1;
				if(i==0 && j==0)
					weightX=1;
				else if(j<=image1.cols/2 && i<=image1.rows/2)
					weightX=1.0-(double)j/(i+j);
				else if(j<=image1.cols/2 && i>image1.rows/2)
					weightX=1.0-(double)j/(j+(image1.rows-i));
				else if(j>image1.cols/2 && i<=image1.rows/2)
					weightX=1.0-(image1.cols-(double)j)/((image1.cols-j)+i);
				else 
					weightX=1.0-(image1.cols-(double)j)/((image1.cols-j)+(image1.rows-i));

				outputImage.at<ushort>(i,j)=/*255*weightX;*/tmpImageX.at<ushort>(i,j)*weightX+tmpImageY.at<ushort>(i,j)*(1-weightX);
			}
		}
		return outputImage;
}



void AlphaBlender::performBlendX(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage){
	double alpha=1,beta=0;
	for(int i=0;i<image1.cols;i++){

		beta=(double)i/(image1.cols-1);
		alpha=1-beta;
		cv::addWeighted(image1.col(i),alpha,image2.col(i),beta,0,outputImage.col(i));
	}
}

void AlphaBlender::performBlendY(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage){

	double alpha=1,beta=0;
	for(int i=0;i<image1.rows;i++){
		beta=(double)i/(image1.rows-1);
		alpha=1-beta;
		cv::addWeighted(image1.row(i),alpha,image2.row(i),beta,0,outputImage.row(i));
	}
}

void AlphaBlender::levelPixels(cv::Mat& image1, cv::Mat& image2) {
	//int diff=image1.at<ushort>(image1.rows/2,image1.cols/2)-image2.at<ushort>(image2.rows/2,image2.cols/2);
	for(int i=0;i<image1.rows;i++){				
		for(int j=0;j<image1.cols;j++){
			if(image1.at<ushort>(i,j)==0){
				image1.at<ushort>(i,j)=image2.at<ushort>(i,j);//+diff;
			}
			if(image2.at<ushort>(i,j)==0){
				image2.at<ushort>(i,j)=image1.at<ushort>(i,j);//-diff;
			}
		}
	}
	cv::imwrite("output/blend/o_Levelling1.png",image1);
	cv::imwrite("output/blend/o_Levelling2.png",image2);
}