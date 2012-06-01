//#include "Stitching.h"
#include "Warp.h"
#include "AlphaBlender.h"


Stitching::Stitching(cv::Mat floatingImage,
	cv::Mat baseImage,
	cv::Mat homography){
		this->floatingImage=floatingImage;
		this->baseImage=baseImage;
		this->homography=homography;		
}

Stitching::~Stitching(){
}
void Stitching::Stitch(){
	//1.Get the new transformed corners and rotated image
	cv::Point baseCorners[4],floatingCorners[4];
	baseCorners[0]=cv::Point(0,0);
	baseCorners[1]=cv::Point(this->baseImage.cols,0);
	baseCorners[2]=cv::Point(this->baseImage.cols,this->baseImage.rows);
	baseCorners[3]=cv::Point(0,this->baseImage.rows);

	floatingCorners[0]=cv::Point(0,0);
	floatingCorners[1]=cv::Point(this->floatingImage.cols,0);
	floatingCorners[2]=cv::Point(this->floatingImage.cols,this->floatingImage.rows);
	floatingCorners[3]=cv::Point(0,this->floatingImage.rows);

	Warp warp;	
	//double tic=cv::getTickCount();
	warp.TransformCorners(floatingCorners,floatingCorners,homography);
	
	printf("Floating Corners Transformation:\n");
	for(int i=0;i<4;i++){
		printf("%i point base: x=%d,y=%d floating:x=%d y=%d\n",
			i,baseCorners[i].x,baseCorners[i].y,
			floatingCorners[i].x,floatingCorners[i].y);
	}

	
	//#### Testing for performance
	//printf("Time Taken=%f Seconds",(cv::getTickCount()-tic)/cv::getTickFrequency());
	/*for(int i=0;i<4;i++){
		printf("\n1st  point %d x=%d y=%d",i,floatingCorners[i].x,floatingCorners[i].y);
	}*/
	warp.RotateImage(this->floatingImage,this->rotatedImage,homography);
	printf("\nrotated Image size: width=%d, height=%d",this->rotatedImage.cols,this->rotatedImage.rows);
	/*
	tic=cv::getTickCount();
	for(int i=0;i<4;i++){
		warp.TransformPoint(*(baseCorners+i),*(floatingCorners+i),&homography);
	}
	printf("Time Taken=%f Seconds",(cv::getTickCount()-tic)/cv::getTickFrequency());
	for(int i=0;i<4;i++){
		printf("\nPoint %d X=%d Y=%d",i,floatingCorners[i].x,floatingCorners[i].y);
	}*/


	//2.Calculate the size of the combined image
	//get minimum x, maximum x, minimum y, maximum y	
	Boundry left,top,right,bottom;
	//Initialisation
	//left.Index=0;right.Index=0;left.Value=right.Value=floatingCorners[0].x;
	//top.Index=0; bottom.Index=0; top.Value=bottom.Value=floatingCorners[0].y;
	//for(int i=0;i<4;i++){
	//	//Get maximum and minimum x values
	//	if(floatingCorners[i].x<baseCorners[i].x){
	//		if(floatingCorners[i].x<left.Value){
	//			left.Index=0;
	//			left.Value=floatingCorners[i].x;
	//		}
	//		if(baseCorners[i].x>right.Value){
	//			right.Index=1;
	//			right.Value=baseCorners[i].x;
	//		}
	//	}else{
	//		if(baseCorners[i].x<left.Value){
	//			left.Index=1;
	//			left.Value=baseCorners[i].x;
	//		}
	//		if(floatingCorners[i].x>right.Value){
	//			right.Index=0;
	//			right.Value=floatingCorners[i].x;
	//		}
	//	}

	//	//Get maximum and minimum y values
	//	if(floatingCorners[i].y<baseCorners[i].y){
	//		if(floatingCorners[i].y<top.Value){
	//			top.Index=0;
	//			top.Value=floatingCorners[i].y;
	//		}
	//		if(baseCorners[i].y>bottom.Value){
	//			bottom.Index=1;
	//			bottom.Value=baseCorners[i].y;
	//		}
	//	}else{
	//		if(baseCorners[i].y<top.Value){
	//			top.Index=1;
	//			top.Value=baseCorners[i].y;
	//		}
	//		if(floatingCorners[i].y>bottom.Value){
	//			bottom.Index=0;
	//			bottom.Value=floatingCorners[i].y;
	//		}
	//	}
	//}
	//printf("Combined boundary: left=%d,top=%d,bottom=%d, right=%d",
	//	left.Value,top.Value,bottom.Value,right.Value);


	//next method to calculate
	int image1Left=floatingCorners[0].x, image1Top=floatingCorners[0].y,
		image1Right=floatingCorners[0].x, image1Bottom=floatingCorners[0].y;

	//printf("\nImage 1:Left=%d, Top=%d, Right=%d, Bottom=%d",image1Left,image1Top,image1Right,image1Bottom);
	
	int image2Left=0,image2Top=0,image2Right=baseImage.cols,image2Bottom=baseImage.rows;
	for(int i=0;i<4;i++){
		if(floatingCorners[i].x<image1Left){
			image1Left=floatingCorners[i].x;
		}
		if(floatingCorners[i].x>image1Right){
			image1Right=floatingCorners[i].x;
		}
		if(floatingCorners[i].y<image1Top){
			image1Top=floatingCorners[i].y;
		}
		if(floatingCorners[i].y>image1Bottom){
			image1Bottom=floatingCorners[i].y;
		}
	}
	image1Right=image1Left+this->rotatedImage.cols;
	image1Bottom=image1Top+this->rotatedImage.rows;
	//printf("\nImage 1:Left=%d, Top=%d, Right=%d, Bottom=%d",image1Left,image1Top,image1Right,image1Bottom);
	//printf("\nImage 2:Left=%d, Top=%d, Right=%d, Bottom=%d",image2Left,image2Top,image2Right,image2Bottom);

	if(image1Left<image2Left){
		left.Index=0;
		left.Value=image1Left;
	}else{
		left.Index=1;
		left.Value=image2Left;
	}
	if(image1Top<image2Top){
		top.Index=0;
		top.Value=image1Top;
	}else{
		top.Index=1;
		top.Value=image2Top;
	}
	if(image1Right>image2Right){
		right.Index=0;
		right.Value=image1Right;
	}else{
		right.Index=1;
		right.Value=image2Right;
	}
	if(image1Bottom>image2Bottom){
		bottom.Index=0;
		bottom.Value=image1Bottom;
	}else{
		bottom.Index=1;
		bottom.Value=image2Bottom;
	}
	printf("Combined boundary: left=%d,top=%d,bottom=%d, right=%d",
		left.Value,top.Value,bottom.Value,right.Value);
	printf("\nRotated Image width=%d, height=%d",this->rotatedImage.cols,this->rotatedImage.rows);

	//preprocessing for stitching
	cv::Rect leftFloatRect, leftStitchRect;
	cv::Rect topFloatRect(0,0,0,0),topStitchRect;
	cv::Rect rightFloatRect,rightStitchRect;
	cv::Rect bottomFloatRect,bottomStitchRect;
	cv::Rect baseRect,baseStitchRect;

	cv::Mat stitchedImage(std::abs(bottom.Value-top.Value)+1,std::abs(right.Value-left.Value)+1,CV_8U);

	//Test of simple method
	cv::Rect floatRegion,baseRegion;
	//common region is the combined region for 
	//both floating and base images
	cv::Rect commonFloatRegion,commonBaseRegion,commonStitchRegion;
	floatRegion.width=this->rotatedImage.cols;
	floatRegion.height=this->rotatedImage.rows;
	baseRegion.width=this->baseImage.cols;
	baseRegion.height=this->baseImage.rows;
	int commonWidth, commonHeight;

	if(left.Index==0){  
		if(top.Index==0){
			printf("case 1");
			floatRegion.x=0;floatRegion.y=0;
			baseRegion.x=std::abs(left.Value);
			baseRegion.y=std::abs(top.Value);	

			
			commonWidth=cv::min(this->rotatedImage.cols-std::abs(left.Value),this->baseImage.cols);		
 			commonHeight=cv::min(this->rotatedImage.rows-std::abs(top.Value),this->baseImage.rows);

			commonFloatRegion.x=std::abs(left.Value);
			commonFloatRegion.y=std::abs(top.Value);
			commonFloatRegion.height=commonHeight;
			commonFloatRegion.width=commonWidth;

			commonBaseRegion.x=0;
			commonBaseRegion.y=0;
			commonBaseRegion.height=commonHeight;
			commonBaseRegion.width=commonWidth;

			commonStitchRegion.x=std::abs(left.Value);
			commonStitchRegion.y=std::abs(top.Value);
			commonStitchRegion.height=commonHeight;
			commonStitchRegion.width=commonWidth;
		}else{
			printf("case 2");
			floatRegion.x=0;floatRegion.y=image1Top;
			baseRegion.x=std::abs(left.Value);
			baseRegion.y=0;

			commonWidth=cv::min(this->rotatedImage.cols-std::abs(left.Value),this->baseImage.cols);
			commonHeight=cv::min(this->rotatedImage.rows,this->baseImage.rows-image1Top);

			commonFloatRegion.x=std::abs(left.Value);
			commonFloatRegion.y=0;
			commonFloatRegion.width=commonWidth;
			commonFloatRegion.height=commonHeight;

			commonBaseRegion.x=0;
			commonBaseRegion.y=image1Top;
			commonBaseRegion.width=commonWidth;
			commonBaseRegion.height=commonHeight;

			commonStitchRegion.x=std::abs(left.Value);
			commonStitchRegion.y=image1Top;
			commonStitchRegion.width=commonWidth;
			commonStitchRegion.height=commonHeight;
		}
	}else{
		if(top.Index==0){
			printf("case 3");
			floatRegion.x=image1Left;floatRegion.y=0;
			baseRegion.x=0;
			baseRegion.y=std::abs(image1Top);

			commonWidth=cv::min(this->baseImage.cols-image1Left,this->rotatedImage.cols);
			commonHeight=cv::min(this->rotatedImage.rows-std::abs(top.Value),this->baseImage.rows);

			commonFloatRegion.x=0;
			commonFloatRegion.y=std::abs(top.Value);
			commonFloatRegion.width=commonWidth;
			commonFloatRegion.height=commonHeight;

			commonBaseRegion.x=image1Left;
			commonBaseRegion.y=0;
			commonBaseRegion.width=commonWidth;
			commonBaseRegion.height=commonHeight;


			commonStitchRegion.x=image1Left;
			commonStitchRegion.y=std::abs(top.Value);
			commonStitchRegion.width=commonWidth;
			commonStitchRegion.height=commonHeight;			
		}else{
			printf("case 4");
			floatRegion.x=image1Left;floatRegion.y=image1Top;
			baseRegion.x=0;
			baseRegion.y=0;			

			commonWidth=cv::min(this->baseImage.cols-image1Left,this->rotatedImage.cols);
			commonHeight=cv::min(this->baseImage.rows-image1Top,this->rotatedImage.rows);

			commonFloatRegion.x=0;
			commonFloatRegion.y=0;
			commonFloatRegion.width=commonWidth;
			commonFloatRegion.height=commonHeight;
			
			
			commonBaseRegion.x=image1Left;
			commonBaseRegion.y=image1Top;
			commonBaseRegion.width=commonWidth;
			commonBaseRegion.height=commonHeight;

			commonStitchRegion.x=image1Left;
			commonStitchRegion.y=image1Top;
			commonStitchRegion.width=commonWidth;
			commonStitchRegion.height=commonHeight;
		}
	}
	
	

	this->rotatedImage.copyTo(stitchedImage(floatRegion));
	this->baseImage.copyTo(stitchedImage(baseRegion));
	cv::imshow("Raw Joined Image", stitchedImage);
	cv::waitKey(0);
	//cv::addWeighted(this->rotatedImage(commonFloatRegion),0.5,this->baseImage(commonBaseRegion),0.5,0,stitchedImage(commonStitchRegion));
	cv::imwrite("output/o_common_float.png",this->rotatedImage(commonFloatRegion));
	cv::imwrite("output/o_common_base.png",this->baseImage(commonBaseRegion));

	/*AlphaBlender blender;
	Mat result=cv::Mat(commonFloatRegion.height,commonFloatRegion.width,CV_8U);
	blender.blend(this->rotatedImage(commonFloatRegion),
		this->baseImage(commonBaseRegion),left,top,right,bottom,result);*/
	/*result.copyTo(stitchedImage(commonStitchRegion));

	cv::imwrite("output/o_stitched.png",stitchedImage);
	cv::imshow("stitchedImage",stitchedImage);
	cv::waitKey(0);*/


	//sample test 
	/*cv::Mat l8u = cv::imread("output/left.png");	
	cv::Mat r8u = cv::imread("output/right.png");*/
	
   /* cv::Mat_<cv::Vec3f> l; l8u.convertTo(l,CV_32F,1.0/255.0);
    cv::Mat_<cv::Vec3f> r; r8u.convertTo(r,CV_32F,1.0/255.0);
 
    cv::Mat_<float> m(l.rows,l.cols,0.0);
    m(cv::Range::all(),cv::Range(0,m.cols/2)) = 1.0;*/

	////blendMask.create(left.rows,left.cols);
	//for(int i=0;i<l.cols;i++){
	//	float alpha=1.0-(float)i/(l.cols-1);
	//	//printf("\taplha=%f",alpha);
	//	//blendMask(cv::Range::all(),cv::Range(i,i))=alpha;
	//	for(int j=0;j<l.rows;j++){
	//		m.at<float>(j,i)=alpha;
	//	}
	//}
 
   /*cv::Mat_<cv::Vec3f> blend = LaplacianBlend(l, r, m);
   cv::imshow("blended",blend);
   cv::waitKey(0);*/
	LaplacianBlender blender(this->rotatedImage(commonFloatRegion),this->baseImage(commonBaseRegion));
	cv::Mat outputImage;
	blender.blend(left,top,right,bottom,outputImage);
   /*blend.copyTo(stitchedImage(commonStitchRegion));
   stitchedImage(commonStitchRegion)=blend;
   cv::imwrite("output/o_stitched_pyr.png",stitchedImage);
   cv::imshow("StitchedImage",stitchedImage);
   cv::waitKey(0);*/


	//3.Get LEFT ROI
	//Logic: left image area, if it lies in the non-overlapping portion,
	//is copied and pasted in the stitched image. if left is of image1, then 
	//we select column range from 0 to absolute left.value. y is always 0 to 
	//image1Bottom
	/*leftFloatRect.x=0;leftFloatRect.y=0;
	leftFloatRect.width=left.Index==0?std::abs(left.Value):0;	
	leftFloatRect.height=this->rotatedImage.rows;

	leftStitchRect.x=0;	leftStitchRect.y=top.Index==0?0:image1Top;
	leftStitchRect.width=left.Index==0?std::abs(left.Value):0;
	leftStitchRect.height=this->rotatedImage.rows;
    
	cv::Mat leftROI=this->rotatedImage(leftFloatRect);
	leftROI.copyTo(stitchedImage(leftStitchRect));	

	cv::imshow("stitchedImage",stitchedImage);
	cv::waitKey(0);*/

	/*cv::imshow("rotatedImage",this->rotatedImage);
	cv::waitKey(0);*/

	//4.Get RIGHT ROI
	//5.Get TOP ROI
	//The top ROI only exists if top.index=0, 
	/*if(top.Index==0){
		topFloatRect.x=std::abs(image1Left);
		topFloatRect.y=0;
		topFloatRect.width=this->rotatedImage.cols-std::abs(image1Left)>baseImage.cols?
			baseImage.cols:this->rotatedImage.cols-std::abs(image1Left);
		topFloatRect.height=std::abs(top.Value);
	}*/
	//6.Get BOTTOM ROI
	//7.Join ROIS to the base image
	//8.Get overlapped ROI and blend
	//9.Copy the nonoverlaped base image content
}







	


	/*for(int i=0;i<image1.rows;i++){
		uchar* data1=image1.ptr<uchar>(i);
		uchar* data2=image2.ptr<uchar>(i);
		for(int j=0;j<image2.cols;j++){
			if(data1[j]==0){				
				data1[j]==data2[j];
				printf("row[%d][%d] data1 Value=%d set!\t",i,j,data2[j]);
			}
			if(data2[j]==0){
				printf("row[%d][%d] data2 Value=%d set!\t",i,j,data1[j]);
				data2[j]=data1[j];
			}
		}
	}*/


// Perform the laplacian blending of two images. 
//The order matters(?) 
//cv::Mat_<cv::Vec3f> Stitching::LaplacianBlend(const cv::Mat_<cv::Vec3f>& l, const cv::Mat_<cv::Vec3f>& r, const cv::Mat_<float>& m) {
//    LaplacianBlending lb(l,r,m,4);
//    return lb.blend();
//}






