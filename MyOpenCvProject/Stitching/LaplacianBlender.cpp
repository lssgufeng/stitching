#include"LaplacianBlender.h"

LaplacianBlender::LaplacianBlender(const cv::Mat& floatImage,const cv::Mat& baseImage){
	cv::Mat image1=floatImage.clone();
	cv::Mat image2=baseImage.clone();
	this->levelPixels(image1,image2);
	cv::cvtColor(image1,this->floatImage,CV_GRAY2BGR);
	cv::cvtColor(image2,this->baseImage,CV_GRAY2BGR);
	this->floatImage.convertTo(this->floatImage,CV_32F,1.0/65535.0);
	this->baseImage.convertTo(this->baseImage,CV_32F,1.0/65535.0);
	this->levels=4;
}

cv::Mat LaplacianBlender::blend(Boundry& left,Boundry& top,Boundry& right,Boundry& bottom){
	this->generateLaplacianPyramid(this->floatImage,this->floatLapPyr,this->floatSmallestLevel);
	this->generateLaplacianPyramid(this->baseImage,this->baseLapPyr, this->baseSmallestLevel);
	//cv::imshow("Float Image", this->floatImage);
	//cv::waitKey(0);
	//cv::imshow("Base Image", this->baseImage);
	//cv::waitKey(0);
	
	//Create Gaussian Pyramids Here
	this->loadBlendMasks();
	this->generateGaussianPyramid(this->blendMaskX,this->maskGaussianPyramidX);
	this->generateGaussianPyramid(this->blendMaskY,this->maskGaussianPyramidY);
	//left implies float image, right implies base image
	if(left.Index==0){
		if(right.Index==0){
			this->blendLapPyrsX(this->floatLapPyr,this->floatLapPyr,this->floatSmallestLevel,this->floatSmallestLevel);
		}else{
			this->blendLapPyrsX(this->floatLapPyr,this->baseLapPyr,this->floatSmallestLevel,this->baseSmallestLevel);
		}
		/*if(top.Index==0){					
			this->blendLapPyrsY(this->floatLapPyr,this->baseLapPyr,this->floatSmallestLevel,this->baseSmallestLevel);
		}else{
			this->blendLapPyrsY(this->baseLapPyr,this->floatLapPyr,this->baseSmallestLevel,this->floatSmallestLevel);
		}*/
	}else{
		if(right.Index==0){
			this->blendLapPyrsX(this->baseLapPyr,this->floatLapPyr,this->baseSmallestLevel,this->floatSmallestLevel);
		}else{
			this->blendLapPyrsX(this->baseLapPyr,this->baseLapPyr,this->baseSmallestLevel,this->baseSmallestLevel);
		}
		/*if(top.Index==0){
			this->blendLapPyrsY(this->floatLapPyr,this->baseLapPyr,this->floatSmallestLevel,this->baseSmallestLevel);
		}else{
			this->blendLapPyrsY(this->baseLapPyr,this->floatLapPyr,this->baseSmallestLevel,this->floatSmallestLevel);
		}*/
	}

	if(top.Index==0){
		if(bottom.Index==0){
			this->blendLapPyrsY(this->floatLapPyr,this->floatLapPyr,this->floatSmallestLevel,this->floatSmallestLevel);
		}else{
			this->blendLapPyrsY(this->floatLapPyr,this->baseLapPyr,this->floatSmallestLevel,this->baseSmallestLevel);
		}
	}else{
		if(bottom.Index==0){
			this->blendLapPyrsY(this->baseLapPyr,this->floatLapPyr,this->baseSmallestLevel,this->floatSmallestLevel);
		}else{
			this->blendLapPyrsY(this->baseLapPyr,this->baseLapPyr,this->baseSmallestLevel,this->baseSmallestLevel);
		}
	}

	//Now we reconstruct the image using result pyramids
	cv::Mat_<cv::Vec3f> blendX,blendY;
	cv::Mat_<cv::Vec3f> result(this->floatImage.rows,this->floatImage.cols);
	blendX=this->reconstructImageX();
	blendY=this->reconstructImageY();
	//result=blendX.clone();			
	/*cv::imshow("ImageX",blendX);
	cv::waitKey(0);
	cv::imshow("imageY",blendY);
	cv::waitKey(0);			*/
	//outputImage=blendX.clone();
	cv::cvtColor(result,result,CV_BGR2GRAY);
	result.convertTo(result,CV_16U,65535);
	cv::cvtColor(blendX,blendX,CV_BGR2GRAY);
	cv::cvtColor(blendY,blendY,CV_BGR2GRAY);
	blendX.convertTo(blendX,CV_16U,65535);
	blendY.convertTo(blendY,CV_16U,65535);
	//Now get the resultant blended image
	for(int i=0;i<this->floatImage.rows;i++){				
		for(int j=0;j<this->floatImage.cols;j++){
			double weightX=-1;
			if(i==0 && j==0)
				weightX=1;
			else if(j<=this->floatImage.cols/2 && i<=this->floatImage.rows/2)
				weightX=1.0-(double)j/(i+j);
			else if(j<=this->floatImage.cols/2 && i>this->floatImage.rows/2)
				weightX=1.0-(double)j/(j+(this->floatImage.rows-i));
			else if(j>this->floatImage.cols/2 && i<=this->floatImage.rows/2)
				weightX=1.0-(this->floatImage.cols-(double)j)/((this->floatImage.cols-(double)j)+(double)i);
			else 
				weightX=1.0-(this->floatImage.cols-(double)j)/((this->floatImage.cols-(double)j)+(this->floatImage.rows-(double)i));
			//printf("i=%d,j=%d,weightX=%f\t",i,j,weightX);
			result.at<ushort>(i,j)=/*255*weightX;*/blendX.at<ushort>(i,j)*weightX+blendY.at<ushort>(i,j)*(1-weightX);
		}
	}
	/*cv::cvtColor(result,result,CV_BGR2GRAY);
	result.convertTo(result,CV_8U,255);*/
	//cv::imshow("OutputImage",result);
	//cv::waitKey(0);
	return result;
}

void LaplacianBlender::loadBlendMasks(){
	//BlendMaskX
	this->blendMaskX.create(this->baseImage.rows,this->baseImage.cols);
	for(int i=0;i<this->baseImage.cols;i++){
		float alpha=1.0-(float)i/(this->baseImage.cols-1);
		//printf("\taplha=%f",alpha);
		//blendMask(cv::Range::all(),cv::Range(i,i))=alpha;
		for(int j=0;j<this->baseImage.rows;j++){
			this->blendMaskX.at<float>(j,i)=alpha;
		}
	}
	//BlendMaskY
	this->blendMaskY.create(this->baseImage.rows,this->baseImage.cols);
	for(int i=0;i<this->baseImage.rows;i++){
		float alpha=1.0-(float)i/(this->baseImage.rows-1);
		//printf("\taplha=%f",alpha);
		//blendMask(cv::Range::all(),cv::Range(i,i))=alpha;
		for(int j=0;j<this->baseImage.cols;j++){
			this->blendMaskY.at<float>(i,j)=alpha;
		}
	}
}



void LaplacianBlender::generateLaplacianPyramid(const cv::Mat& image,cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr,
		cv::Mat& smallestLevel){
			lapPyr.clear();
			cv::Mat currentImage=image.clone();
			for(int i=0;i<levels;i++){
				cv::Mat down, up;
				cv::pyrDown(currentImage,down);
				cv::pyrUp(down, up,currentImage.size());
				cv::Mat_<cv::Vec3f> lap=currentImage-up;
				lapPyr.push_back(lap);
				currentImage=down;
			}
			currentImage.copyTo(smallestLevel);
			//cv::imshow("Smallest Level",smallestLevel);
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
		/*cv::imshow(message,_down);*/
		/*cv::waitKey(0);*/
		cv::Mat down;
		cv::cvtColor(_down,down,CV_GRAY2BGR);
		maskGaussianPyramid.push_back(down);
		currentImage=_down;
	}
}

void LaplacianBlender::blendLapPyrsX(cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr1,cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr2,
	cv::Mat& smallestLevel1,cv::Mat& smallestLevel2){
		printf("Smallest Level1 Size:row=%d, column=%d\t Mask gauss. Size row=%d col=%d",
			smallestLevel1.rows,smallestLevel1.cols,maskGaussianPyramidX.back().rows,maskGaussianPyramidX.back().cols); 

		printf("Smallest Level2 Size:row=%d, column=%d\t Mask gauss. Size row=%d col=%d",
			smallestLevel2.rows,smallestLevel2.cols,maskGaussianPyramidX.back().rows,maskGaussianPyramidX.back().cols); 

		this->resultSmallestLevelX=smallestLevel1.mul(this->maskGaussianPyramidX.back())+
			smallestLevel2.mul(cv::Scalar(1.0,1.0,1.0)-maskGaussianPyramidX.back());
		//cv::multiply(leftSmallestLevel,maskGaussianPyramid.back(),resultSmallestLevel);
		for(int i=0;i<this->levels;i++){
			printf("Lap Pyramid Size:row=%d, column=%d\t Mask gauss. Size row=%d col=%d",
				lapPyr1[i].rows,lapPyr1[i].cols,maskGaussianPyramidX[i].rows,maskGaussianPyramidX[i].cols); 

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
			cv::Mat_<cv::Vec3f> A=lapPyr1[i].mul(this->maskGaussianPyramidY[i]);
			cv::Mat antiMask=cv::Scalar(1.0,1.0,1.0)-maskGaussianPyramidY[i];
			cv::Mat_<cv::Vec3f> B=lapPyr2[i].mul(antiMask);
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
	//cv::cvtColor(currentImage,currentImage,CV_BGR2GRAY);
	return currentImage;
}

cv::Mat LaplacianBlender::reconstructImageY(){
	cv::Mat currentImage=this->resultSmallestLevelY;
	for(int i=levels-1; i>=0;i--){
		cv::Mat up;
		cv::pyrUp(currentImage,up,this->resultLapPyrY[i].size());
		currentImage=up+resultLapPyrY[i];
	 }
	//cv::cvtColor(currentImage,currentImage,CV_BGR2GRAY);
	return currentImage;
}

void LaplacianBlender::levelPixels(cv::Mat& image1, cv::Mat& image2) {
	for(int i=0;i<image1.rows;i++){
		for(int j=0;j<image1.cols;j++){
			if(image1.at<ushort>(i,j)==0){
				image1.at<ushort>(i,j)=image2.at<ushort>(i,j);
			}
			if(image2.at<ushort>(i,j)==0){
				image2.at<ushort>(i,j)=image1.at<ushort>(i,j);
			}
		}
	}
	cv::medianBlur(image1,image1,3);
	cv::medianBlur(image2,image2,3);
	//cv::imwrite("output/blend/o_Levelling1.png",image1);
	//cv::imwrite("output/blend/o_Levelling2.png",image2);
}






