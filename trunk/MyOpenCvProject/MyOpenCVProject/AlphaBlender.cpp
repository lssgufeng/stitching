#include "AlphaBlender.h"

cv::Mat AlphaBlender::blend(cv::Mat& image1,cv::Mat& image2,
	Boundry& left,Boundry& top,Boundry& right,Boundry& bottom,
	cv::Mat outputImage){
		this->levelPixels(image1,image2);
		cv::Mat tmpImageX, tmpImageY;
        //X-direction
		if(left.Index==0){
			if(right.Index=0){
				printf("Blend case 1");
				performBlendX(image1,image1,tmpImageX);
			}else{
				printf("Blend case 2");
				performBlendX(image1,image2,tmpImageX);
			}
		}else{
			if(right.Index==0){
				printf("Blend case 3");
				performBlendX(image2,image1,tmpImageX);
			}else{
				printf("Blend case 4");
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
		
		//Join the images
		cv::addWeighted(tmpImageX,0.5,tmpImageY,0.5,0,outputImage);
		cv::imwrite("output/o_output_blend.png",outputImage);
		cv::imshow("output Image", outputImage);
		return outputImage;
}

void AlphaBlender::performBlendX(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage){
	/*cv::imshow("BlendX:image1",image1);
	cv::waitKey(0);

	cv::imshow("BlendX:image2",image2);
	cv::waitKey(0);*/

	//cv::addWeighted(image1,0.5,image2,0.5,0,outputImage);

	double alpha=1,beta=0;
	for(int i=0;i<image1.cols;i++){
		beta=(double)i/(image1.cols-1);
		alpha=1-beta;
		cv::addWeighted(image1.col(i),alpha,image2.col(i),beta,0,outputImage.col(i));
	}
	//outputImage=cv::max(image1,image2);
	cv::imshow("blendX",outputImage);
	cv::imwrite("output/blend/blendX.png",outputImage);
	cv::waitKey(0);
}

void AlphaBlender::performBlendY(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage){
	/*cv::imshow("BlendY:image1",image1);
	cv::waitKey(0);

	cv::imshow("BlendY:image2",image2);
	cv::waitKey(0);*/
	cv::addWeighted(image1,0,image2,9,0,outputImage);

	double alpha=1,beta=0;
	for(int i=0;i<image1.rows;i++){
		beta=(double)i/(image1.rows-1);
		alpha=1-beta;
		cv::addWeighted(image1.row(i),alpha,image2.row(i),1-alpha,0,outputImage.row(i));
	}
	cv::imshow("blendY",outputImage);
	cv::imwrite("output/blend/blendY.png",outputImage);
	cv::waitKey(0);
}

void AlphaBlender::levelPixels(cv::Mat& image1, cv::Mat& image2) {
	for(int i=0;i<image1.rows;i++){
		for(int j=0;j<image1.cols;j++){
			if(image1.at<uchar>(i,j)==0){
				image1.at<uchar>(i,j)=image2.at<uchar>(i,j);
			}
			if(image2.at<uchar>(i,j)==0){
				image2.at<uchar>(i,j)=image1.at<uchar>(i,j);
			}
		}
	}
}