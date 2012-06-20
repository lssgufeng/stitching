#include "AlphaBlender.h"

cv::Mat AlphaBlender::blend(const cv::Mat& firstImage,const cv::Mat& secondImage,
	Boundry& left,Boundry& top,Boundry& right,Boundry& bottom,
	cv::Mat& outputImage){
		cv::Mat image1=firstImage.clone();
		cv::Mat image2=secondImage.clone();
		this->levelPixels(image1,image2);
		cv::imwrite("output/left.png",image1);
		cv::imwrite("output/right.png",image2);
		cv::Mat tmpImageX(image1.rows,image2.cols,CV_16U), tmpImageY(image1.rows,image1.cols,CV_16U);
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

				//printf("i=%d,j=%d,weightX=%f\t",i,j,weightX);
				outputImage.at<ushort>(i,j)=/*255*weightX;*/tmpImageX.at<ushort>(i,j)*weightX+tmpImageY.at<ushort>(i,j)*(1-weightX);
			}
		}
		/*cv::medianBlur(outputImage,outputImage,3);*/
		/*outputImage=tmpImageX.clone();*/
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
	//cv::medianBlur(outputImage,outputImage,5);
	cv::imshow("blendX",outputImage);
	cv::imwrite("output/blend/blendX.png",outputImage);
	cv::waitKey(0);
}

void AlphaBlender::performBlendY(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage){
	/*cv::imshow("BlendY:image1",image1);
	cv::waitKey(0);

	cv::imshow("BlendY:image2",image2);
	cv::waitKey(0);*/
	//cv::addWeighted(image1,0,image2,9,0,outputImage);

	double alpha=1,beta=0;
	for(int i=0;i<image1.rows;i++){
		beta=(double)i/(image1.rows-1);
		alpha=1-beta;
		cv::addWeighted(image1.row(i),alpha,image2.row(i),beta,0,outputImage.row(i));
	}
	//cv::medianBlur(outputImage,outputImage,3);
	cv::imshow("blendY",outputImage);
	cv::imwrite("output/blend/blendY.png",outputImage);
	cv::waitKey(0);
}

void AlphaBlender::levelPixels(cv::Mat& image1, cv::Mat& image2) {
	int diff=image1.at<ushort>(image1.rows/2,image1.cols/2)-image2.at<ushort>(image2.rows/2,image2.cols/2);
	for(int i=0;i<image1.rows;i++){				
		for(int j=0;j<image1.cols;j++){
			if(image1.at<ushort>(i,j)==0){
				image1.at<ushort>(i,j)=image2.at<ushort>(i,j)+diff;
			}
			if(image2.at<ushort>(i,j)==0){
				image2.at<ushort>(i,j)=image1.at<ushort>(i,j)-diff;
			}
			/*if(image1.at<ushort>(i,j)!=0 && image2.at<ushort>(i,j)==0){
				diff=image1.at<ushort>(i,j)-image2.at<ushort>(i,j);
			}*/
		}
	}
	cv::medianBlur(image1,image1,3);
	cv::medianBlur(image2,image2,3);
	cv::imwrite("output/blend/o_Levelling1.png",image1);
	cv::imwrite("output/blend/o_Levelling2.png",image2);
}