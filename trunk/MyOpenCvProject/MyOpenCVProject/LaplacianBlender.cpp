#include"LaplacianBlender.h"

LaplacianBlender::LaplacianBlender(const cv::Mat& floatImage,const cv::Mat& baseImage){
	floatImage.convertTo(this->floatImage,CV_32F);
	baseImage.convertTo(this->baseImage,CV_32F);
	this->levels=4;
}

void LaplacianBlender::blend(Boundry& left,Boundry& top,Boundry& right,Boundry& bottom,
		cv::Mat& outputImage){
			cv::Mat leftImage, rightImage, topImage, bottomImage;
			if(left.Index==0){
				if(top.Index==0){
					leftImage=topImage=this->floatImage.clone();
					rightImage=bottomImage= this->baseImage.clone();					
				}else{
					leftImage=bottomImage=this->floatImage.clone();
					rightImage=topImage= this->baseImage.clone();
				}
			}else{
				if(top.Index==0){
					rightImage=topImage=this->floatImage.clone();
					leftImage=bottomImage=this->baseImage.clone();
				}else{
					rightImage=bottomImage=this->floatImage.clone();
					leftImage=topImage=this->baseImage.clone();
				}
			}

			this->generateLaplacianPyramid(leftImage,this->,this->sma
}


void LaplacianBlender::generateLaplacianPyramid(const cv::Mat& image,cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr,
		cv::Mat& smallestLevel){
			lapPyr.clear();
			cv::Mat currentImage=image.clone();
			for(int i=0;i<levels;i++){
				cv::Mat down, up;
				cv::pyrDown(currentImage,down);
				cv::pyrUp(down, up,currentImage.size());
				cv::Mat lap=currentImage-up;
				lapPyr.push_back(lap);
				currentImage=down;
			}
			currentImage.copyTo(smallestLevel);
			cv::imshow("Smallest Level",smallestLevel);
			cv::waitKey(0);
}

void LaplacianBlender::generateGaussianPyramid(cv::Mat& blendMask,
	cv::Vector<cv::Mat_<cv::Vec3f>>& maskGaussianPyramid){
	assert(this->floatLapPyr.size()>0);

	maskGaussianPyramid.clear();
	cv::Mat currentImage;
	cv::cvtColor(blendMask,currentImage,CV_GRAY2BGR);
	maskGaussianPyramid.push_back(currentImage);
	currentImage=blendMask;

	for(int i=1;i<this->levels+1;i++){
		cv::Mat _down;
		if(i<this->floatLapPyr.size()){
			cv::pyrDown(currentImage,_down,this->floatLapPyr[i].size());
		}else{
			cv::pyrDown(currentImage,_down,this->floatSmallestLevel.size());
		}
		char message[100];
		sprintf(message,"Gaussian Mask: Level %d",i);
		cv::imshow(message,_down);
		cv::waitKey(0);
		cv::Mat down;
		cv::cvtColor(_down,down,CV_GRAY2BGR);
		maskGaussianPyramid.push_back(down);
		currentImage=_down;
	}
}

void LaplacianBlender::blendLapPyrsX(cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr1,cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr2,
	cv::Mat& smallestLevel1,cv::Mat& smallestLevel2){
		this->resultSmallestLevelX=smallestLevel1.mul(this->maskGaussianPyramidX.back())+
			smallestLevel2.mul(cv::Scalar(1.0,1.0,1.0)-maskGaussianPyramidX.back());
		//cv::multiply(leftSmallestLevel,maskGaussianPyramid.back(),resultSmallestLevel);
		for(int i=0;i<this->levels;i++){
			cv::Mat A=lapPyr1[i].mul(this->maskGaussianPyramidX[i]);
			cv::Mat antiMask=cv::Scalar(1.0,1.0,1.0)-maskGaussianPyramidX[i];
			cv::Mat B=lapPyr2[i].mul(antiMask);
			cv::Mat_<cv::Vec3f> result=A+B;
			this->resultLapPyrX.push_back(result);		
		}
}

void LaplacianBlender::blendLapPyrsY(cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr1,cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr2,
	cv::Mat& smallestLevel1,cv::Mat& smallestLevel2){
		this->resultSmallestLevelY=smallestLevel1.mul(this->maskGaussianPyramidY.back())+
			smallestLevel2.mul(cv::Scalar(1.0,1.0,1.0)-maskGaussianPyramidY.back());
		//cv::multiply(leftSmallestLevel,maskGaussianPyramid.back(),resultSmallestLevel);
		for(int i=0;i<this->levels;i++){
			cv::Mat A=lapPyr1[i].mul(this->maskGaussianPyramidY[i]);
			cv::Mat antiMask=cv::Scalar(1.0,1.0,1.0)-maskGaussianPyramidY[i];
			cv::Mat B=lapPyr2[i].mul(antiMask);
			cv::Mat_<cv::Vec3f> result=A+B;
			this->resultLapPyrY.push_back(result);
		}
}

cv::Mat LaplacianBlender::reconstructImageX(){
	cv::Mat currentImage=this->resultSmallestLevelX;
	for(int i=levels-1; i>=0;i--){
		cv::Mat up;
		cv::pyrUp(currentImage,up,this->resultLapPyrX[i].size());
		currentImage=up+this->resultLapPyrX[i];
	 }
	return currentImage;
}

cv::Mat LaplacianBlender::reconstructImageY(){
	cv::Mat currentImage=this->resultSmallestLevelY;
	for(int i=levels-1; i>=0;i--){
		cv::Mat up;
		cv::pyrUp(currentImage,up,this->resultLapPyrY[i].size());
		currentImage=up+resultLapPyrY[i];
	 }
	return currentImage;
}






