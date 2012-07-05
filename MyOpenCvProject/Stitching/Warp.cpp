#include "Warp.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
#define PI 3.141592653589793238462643383279502884197 //copied from wolfram:)

Warp::Warp(){
}
Warp::~Warp(){
}
void Warp::GetWarpPoints(cv::Mat& srcPts,
		cv::Mat& dstPts,
		cv::Mat& homography){
			double tic=cv::getTickCount();
			//check feasibility
			if(homography.cols==srcPts.rows){
				dstPts=homography*srcPts;
				for(int i=0;i<dstPts.rows;i++){
					double* data=dstPts.ptr<double>(i);
					double* scale=dstPts.ptr<double>(dstPts.rows-1);
					for(int j=0;j<dstPts.cols;j++){
						*data++=*data / *scale++;
					}			
				}
			}
			printf("\nGetWarpPoints took %f seconds\n",(cv::getTickCount()-tic)/cv::getTickFrequency());
}


void Warp::GetWarpPoint(cv::Mat& homography,
		cv::Point& point,
		cv::Point& dstPoint){
			double tic=cv::getTickCount();
			cv::Mat tmpPointSrc(3,1,CV_64F),tmpPointDst;
			
			tmpPointSrc.at<double>(0,0)=point.x;
			tmpPointSrc.at<double>(0,1)=point.y;
			tmpPointSrc.at<double>(0,2)=1;
			GetWarpPoints(homography,tmpPointSrc,tmpPointDst);
			dstPoint=cv::Point(tmpPointDst.at<double>(0,0),
				tmpPointDst.at<double>(0,1));
			printf("GetWarpPoint took %f seconds\n",(cv::getTickCount()-tic)/cv::getTickFrequency());
}

void Warp::TransformPoint(const cv::Point pointToTransform,
		cv::Point& outputPoint,
		const cv::Mat& homography) {
			double tic=cv::getTickCount();
			double coordinates[3] = {pointToTransform.x, pointToTransform.y, 1};
			cv::Mat originVector = cv::Mat(3, 1, CV_64F, coordinates);
			cv::Mat transformedVector = cv::Mat(3, 1, CV_64F, coordinates);
			//cv::MatMul(matrix, &originVector, &transformedVector);
			transformedVector=homography*originVector;
			std::cout<<"Transformed Vector"<<std::endl;
			std::cout<<transformedVector;
			outputPoint = cv::Point((int)(transformedVector.at<double>(0,0)/ transformedVector.at<double>(2, 0)),
				(int)(transformedVector.at<double>(1,0)/ transformedVector.at<double>(2, 0)));
			std::cout<<"output x"<<outputPoint.x<<"output y="<<outputPoint.y<<std::endl;
			printf("TransformPoint took %f seconds\n",(cv::getTickCount()-tic)/cv::getTickFrequency());
}

void Warp::TransformCorners(const cv::Point* corners,
	cv::Point* outputCorners,
	const cv::Mat homography){
		for(int i=0;i<4;i++){
			TransformPoint(*(corners+i),*(outputCorners+i),homography);
		}
}

void Warp::RotateImage(const cv::Mat image,cv::Mat homography,cv::Mat& outputImage,
	cv::Point& topLeft,cv::Point& bottomRight){
	cv::Mat corners1(1, 4,CV_32F);
	cv:: Mat corners2(1,4,CV_32F);
	cv::Mat corners(1,4,CV_32F);
	cv::vector<cv::Mat> planes;

	corners1.at<float>(0,0)=0;
	corners2.at<float>(0,0)=0;
	corners1.at<float>(0,1)=image.cols;
	corners2.at<float>(0,1)=0;
	corners1.at<float>(0,2)=image.cols;
	corners2.at<float>(0,2)=image.rows;
	corners1.at<float>(0,3)=0;
	corners2.at<float>(0,3)=image.rows;
	planes.push_back(corners1);
	planes.push_back(corners2);

	cv::merge(planes,corners);

	cv::perspectiveTransform(corners, corners, homography);

	this->GetCorners(corners,topLeft,bottomRight); 

	cv::Mat T;
	T=cv::Mat::zeros(3,3,CV_64F);
	T.at<double>(0,0)=1;
	T.at<double>(1,1)=1;
	T.at<double>(2,2)=1;
	T.at<double>(0,2)=-topLeft.x;
	T.at<double>(1,2)=-topLeft.y;

	// change homography to take necessary translation into account
	cv::gemm(T, homography,1,T,0,T);
	// warp second image and copy it to output image
	cv::warpPerspective(image,outputImage, T, 
		cv::Size(bottomRight.x-topLeft.x,bottomRight.y-topLeft.y),
		cv::INTER_LINEAR,0,32768);

	
}

void Warp::RotateImage_Xcrop(cv::Mat image,
	cv::Mat homography, cv::Mat& outputImage, cv::Point& topLeft, cv::Point& bottomRight){
	cv::Mat corners1(1, 4,CV_32F);
	cv:: Mat corners2(1,4,CV_32F);
	cv::Mat corners(1,4,CV_32F);
	cv::vector<cv::Mat> planes;

	corners1.at<float>(0,0)=0;
	corners2.at<float>(0,0)=0;
	corners1.at<float>(0,1)=image.cols;
	corners2.at<float>(0,1)=0;
	corners1.at<float>(0,2)=image.cols;
	corners2.at<float>(0,2)=image.rows;
	corners1.at<float>(0,3)=0;
	corners2.at<float>(0,3)=image.rows;
	planes.push_back(corners1);
	planes.push_back(corners2);

	cv::merge(planes,corners);

	cv::perspectiveTransform(corners, corners, homography);
	this->GetCorners_Xcrop(corners,topLeft,bottomRight); 
	
	cv::Mat T;
	T=cv::Mat::zeros(3,3,CV_64F);
	T.at<double>(0,0)=1;
	T.at<double>(1,1)=1;
	T.at<double>(2,2)=1;
	T.at<double>(0,2)=-topLeft.x;
	T.at<double>(1,2)=-topLeft.y;

	// change homography to take necessary translation into account
	cv::gemm(T, homography,1,T,0,T);
	// warp second image and copy it to output image
	cv::warpPerspective(image,outputImage, T, 
		cv::Size(bottomRight.x-topLeft.x,bottomRight.y-topLeft.y),
		cv::INTER_LINEAR,0,32768);
}

void Warp::RotateImage_Ycrop(cv::Mat image,
	cv::Mat homography, cv::Mat& outputImage,cv::Point& topLeft, cv::Point& bottomRight){
    cv::Mat corners1(1, 4,CV_32F);
	cv:: Mat corners2(1,4,CV_32F);
	cv::Mat corners(1,4,CV_32F);
	cv::vector<cv::Mat> planes;

	corners1.at<float>(0,0)=0;
	corners2.at<float>(0,0)=0;
	corners1.at<float>(0,1)=image.cols;
	corners2.at<float>(0,1)=0;
	corners1.at<float>(0,2)=image.cols;
	corners2.at<float>(0,2)=image.rows;
	corners1.at<float>(0,3)=0;
	corners2.at<float>(0,3)=image.rows;
	planes.push_back(corners1);
	planes.push_back(corners2);

	cv::merge(planes,corners);

	cv::perspectiveTransform(corners, corners, homography);
    this->GetCorners_Ycrop(corners,topLeft,bottomRight); 
	
	cv::Mat T;
	T=cv::Mat::zeros(3,3,CV_64F);
	T.at<double>(0,0)=1;
	T.at<double>(1,1)=1;
	T.at<double>(2,2)=1;
	T.at<double>(0,2)=-topLeft.x;
	T.at<double>(1,2)=-topLeft.y;

	// change homography to take necessary translation into account
	cv::gemm(T, homography,1,T,0,T);
	// warp second image and copy it to output image
	cv::warpPerspective(image,outputImage, T, 
		cv::Size(bottomRight.x-topLeft.x,bottomRight.y-topLeft.y),
		cv::INTER_LINEAR,0,32768);
}

void Warp::TestTransformation(cv::Mat& image,
	double angle_deg,
	double xTrans, 
	double yTrans){
	cv::Mat t(3,3,CV_64F);
	t=0;
	double angle=angle_deg*PI/180;

	t.at<double>(0,0)=cos(angle);
	t.at<double>(1,1)=cos(angle);

	t.at<double>(0,1) = -sin(angle);
	t.at<double>(1,0) = sin(angle);
	
	t.at<double>(0,2) = xTrans;    
	t.at<double>(1,2) = yTrans;
	
	t.at<double>(2,2) = 1;
	t.at<double>(2,0)=t.at<double>(2,1)=0;	

	
	cv::Mat destination;	
	cv::warpPerspective(image,destination,t,image.size());
	char buffer[100];
	sprintf(buffer,"output/Rotation/Rotation_%f.png",angle_deg);
	cv::imwrite(buffer,destination);
	cv::imshow("Rotation",destination);
	cv::waitKey(0);
}

void Warp::GetCustomHomography(double angle, 
		double xTrans, 
		double yTrans,
		cv::Mat& homography){
			angle*=PI/180;
			homography.at<double>(0,0)=homography.at<double>(1,1)=cos(angle);
			homography.at<double>(0,1)=-sin(angle);
			homography.at<double>(1,0)=sin(angle);
			homography.at<double>(0,2) = xTrans;    
			homography.at<double>(1,2) = yTrans;
			homography.at<double>(2,2) = 1;
			homography.at<double>(2,0)=homography.at<double>(2,1)=0;	
}


void Warp::WarpPerspective( const cv::Mat& src, cv::Mat& dst, const cv::Mat& M0, cv::Size dsize,
        int flags, int borderType, const cv::Scalar& borderValue, CvPoint origin )
{
	dst.create( dsize, src.type() );
	
	const int BLOCK_SZ = 32;
	
	short XY[BLOCK_SZ*BLOCK_SZ*2], A[BLOCK_SZ*BLOCK_SZ];
	double M[9];
	cv::Mat _M(3, 3, CV_64F, M);
	int interpolation = flags & cv::INTER_MAX;
	if( interpolation == cv::INTER_AREA )
		interpolation = cv::INTER_LINEAR;

	CV_Assert( (M0.type() == CV_32F || M0.type() == CV_64F) && M0.rows == 3 && M0.cols == 3 );
	M0.convertTo(_M, _M.type());

	if( !(flags & cv::WARP_INVERSE_MAP) )
		invert(_M, _M);

	int x, xDest, y, yDest, x1, y1, width = dst.cols, height = dst.rows;
	
	int bh0 = std::min(BLOCK_SZ/2, height);
	int bw0 = std::min(BLOCK_SZ*BLOCK_SZ/bh0, width);
	bh0 = std::min(BLOCK_SZ*BLOCK_SZ/bw0, height);
	
	for( y = -origin.y, yDest = 0; y < height; y += bh0, yDest += bh0 )
	{
		for( x = -origin.x, xDest = 0; x < width; x += bw0, xDest += bw0 )
		{
			int bw = std::min( bw0, width - xDest);//*/std::min( bw0, width - x);
			int bh = std::min( bh0, height - yDest);//*/std::min( bh0, height - y);
			// to avoid dimensions errors
			if (bw <= 0 || bh <= 0)
				break;

			cv::Mat _XY(bh, bw, CV_16SC2, XY), _A;
			cv::Mat dpart(dst, cv::Rect(xDest, yDest, bw, bh));
			for( y1 = 0; y1 < bh; y1++ )
			{
				short* xy = XY + y1*bw*2;
				double X0 = M[0]*x + M[1]*(y + y1) + M[2];
				double Y0 = M[3]*x + M[4]*(y + y1) + M[5];            
				double W0 = M[6]*x + M[7]*(y + y1) + M[8];
				
				if( interpolation == cv::INTER_NEAREST )
					for( x1 = 0; x1 < bw; x1++ ){
						double W = W0 + M[6]*x1;
						W = W ? 1./W : 0;
						int X = cv::saturate_cast<int>((X0 + M[0]*x1)*W);
						int Y = cv::saturate_cast<int>((Y0 + M[3]*x1)*W);
						xy[x1*2] = (short)X;
						xy[x1*2+1] = (short)Y;
					}else{
						short* alpha = A + y1*bw;
						for( x1 = 0; x1 < bw; x1++ ){
							double W = W0 + M[6]*x1;
							W = W ? cv::INTER_TAB_SIZE/W : 0;
							int X = cv::saturate_cast<int>((X0 + M[0]*x1)*W);
							int Y = cv::saturate_cast<int>((Y0 + M[3]*x1)*W);
							xy[x1*2] = (short)(X >> cv::INTER_BITS) + origin.x;
							xy[x1*2+1] = (short)(Y >> cv::INTER_BITS) + origin.y;
							alpha[x1] = (short)((Y & (cv::INTER_TAB_SIZE-1))*cv::INTER_TAB_SIZE +
								(X & (cv::INTER_TAB_SIZE-1)));
						}
					}
			}
			if( interpolation == cv::INTER_NEAREST )
				remap( src, dpart, _XY,cv::Mat(), interpolation, borderType, borderValue );
			else
			{
				cv::Mat _A(bh, bw, CV_16U, A);
				remap( src, dpart, _XY, _A, interpolation, borderType, borderValue );
			}
		}
	}
}

void Warp::GetCorners(const cv::Point corners[],cv::Point& topLeft,cv::Point& bottomRight){

	int minX=INT_MAX,minY=INT_MAX,maxX=INT_MIN, maxY=INT_MIN;
	for(int i=0;i<4;i++){
		//Get max and min X
		if(corners[i].x<minX){
			minX=corners[i].x;
		}else if(corners[i].x>maxX){
			maxX=corners[i].x;
		}
		
		//Get max and min Y
		if(corners[i].y<minY){
			minY=corners[i].y;
		}else if(corners[i].y>maxY){
			maxY=corners[i].y;
		}
	}
	topLeft.x=minX;
	topLeft.y=minY;
	bottomRight.x=maxX;
	bottomRight.y=maxY;
}

void Warp::GetCorners(const cv::Mat corners,cv::Point& topLeft,cv::Point& bottomRight){
	topLeft.x=cv::min(cv::min((double)corners.at<cv::Vec2f>(0,0)[0],(double)corners.at<cv::Vec2f>(0,1)[0]),
		cv::min((double)corners.at<cv::Vec2f>(0,2)[0],(double)corners.at<cv::Vec2f>(0,3)[0]));
	topLeft.y=cv::min(cv::min((double)corners.at<cv::Vec2f>(0,0)[1],(double)corners.at<cv::Vec2f>(0,1)[1]),
		cv::min((double)corners.at<cv::Vec2f>(0,2)[1],(double)corners.at<cv::Vec2f>(0,3)[1]));

	bottomRight.x=cv::max(cv::max((double)corners.at<cv::Vec2f>(0,0)[0],(double)corners.at<cv::Vec2f>(0,1)[0]),
		cv::max((double)corners.at<cv::Vec2f>(0,2)[0],(double)corners.at<cv::Vec2f>(0,3)[0]));
	bottomRight.y=cv::max(cv::max((double)corners.at<cv::Vec2f>(0,0)[1],(double)corners.at<cv::Vec2f>(0,1)[1]),
		cv::max((double)corners.at<cv::Vec2f>(0,2)[1],(double)corners.at<cv::Vec2f>(0,3)[1]));
}


/*
void Warp::GetMinimalCorners(const cv:: Point corners[], 
	cv::Point& topLeft, cv::Point& bottomRight){
		cv::Point tmpCorners[4]={corners[0],corners[1],corners[2],corners[3]};
		//Bubble Sort: Easy to think 
		for(int i=0;i<4-1;i++){
			for(int j=i+1;j<4;j++){
				if(tmpCorners[j].x<tmpCorners[i].x){
					int tmp;
					tmp=tmpCorners[i].x;
					tmpCorners[i].x=tmpCorners[j].x;
					tmpCorners[j].x=tmp;
				}
				if(tmpCorners[j].y<tmpCorners[i].y){
					int tmp;
					tmp=tmpCorners[i].y;
					tmpCorners[i].y=tmpCorners[j].y;
					tmpCorners[j].y=tmp;
				}
			}
		}
		topLeft.x=tmpCorners[1].x;
		topLeft.y=tmpCorners[1].y;
		bottomRight.x=tmpCorners[2].x;
		bottomRight.y=tmpCorners[2].y;

}
*/

void Warp::GetCorners_Ycrop(const cv::Point corners[], cv::Point& topLeft, cv::Point& bottomRight){
	int yValues[]={corners[0].y,corners[1].y,corners[2].y, corners[3].y};
	int minX=INT_MAX,maxX=INT_MIN;
	for(int i=0;i<3;i++){
		for(int j=i+1;j<4;j++){
			if(yValues[j]<yValues[i]){
				int temp=yValues[i];
				yValues[i]=yValues[j];
				yValues[j]=temp;
			}			
		}
	}
	for(int i=0;i<4;i++){
		if(corners[i].x<minX){
			minX=corners[i].x;
		}
		if(corners[i].x>maxX){
			maxX=corners[i].x;
		}
	}

	topLeft.x=minX;
	topLeft.y=yValues[1];
	bottomRight.x=maxX;
	bottomRight.y=yValues[2];
}

void Warp::GetCorners_Ycrop(const cv::Mat corners,cv::Point& topLeft,cv::Point& bottomRight){
	double yValues[]={(double)corners.at<cv::Vec2f>(0,0)[1],(double)corners.at<cv::Vec2f>(0,1)[1],
		(double)corners.at<cv::Vec2f>(0,2)[1],(double)corners.at<cv::Vec2f>(0,3)[1]};
	double minX=(double)INT_MAX,maxX=(double)INT_MIN;
	for(int i=0;i<3;i++){
		for(int j=i+1;j<4;j++){
			if(yValues[j]<yValues[i]){
				int temp=yValues[i];
				yValues[i]=yValues[j];
				yValues[j]=temp;
			}			
		}
	}
	for(int i=0;i<4;i++){
		if((double)corners.at<cv::Vec2f>(0,i)[0]<minX){
			minX=corners.at<cv::Vec2f>(0,i)[0];
		}
		if((double)corners.at<cv::Vec2f>(0,i)[0]>maxX){
			maxX=corners.at<cv::Vec2f>(0,i)[0];
		}
	}

	topLeft.x=minX;
	topLeft.y=yValues[1];
	bottomRight.x=maxX;
	bottomRight.y=yValues[2];
	
}

void Warp::GetCorners_Xcrop(const cv::Point corners[], cv::Point& topLeft, cv::Point& bottomRight){
	int xValues[]={corners[0].x,corners[1].x,corners[2].x, corners[3].x};
	int minY=INT_MAX,maxY=INT_MIN;
	for(int i=0;i<3;i++){
		for(int j=i+1;j<4;j++){
			if(xValues[j]<xValues[i]){
				int temp=xValues[i];
				xValues[i]=xValues[j];
				xValues[j]=temp;
			}			
		}
	}
	for(int i=0;i<4;i++){
		if(corners[i].y<minY){
			minY=corners[i].y;
		}
		if(corners[i].y>maxY){
			maxY=corners[i].y;
		}
	}

	topLeft.x=xValues[1];
	topLeft.y=minY;
	bottomRight.x=xValues[2];
	bottomRight.y=maxY;	
}

void Warp::GetCorners_Xcrop(const cv::Mat corners,cv::Point& topLeft,cv::Point& bottomRight){
	double xValues[]={(double)corners.at<cv::Vec2f>(0,0)[0],(double)corners.at<cv::Vec2f>(0,1)[0],
		(double)corners.at<cv::Vec2f>(0,2)[0],(double)corners.at<cv::Vec2f>(0,3)[0]};
	double minY=(double)INT_MAX,maxY=(double)INT_MIN;
	for(int i=0;i<3;i++){
		for(int j=i+1;j<4;j++){
			if(xValues[j]<xValues[i]){
				int temp=xValues[i];
				xValues[i]=xValues[j];
				xValues[j]=temp;
			}			
		}
	}
	for(int i=0;i<4;i++){
		if((double)corners.at<cv::Vec2f>(0,i)[1]<minY){
			minY=(double)corners.at<cv::Vec2f>(0,i)[1];
		}
		if((double)corners.at<cv::Vec2f>(0,i)[1]>maxY){
			maxY=(double)corners.at<cv::Vec2f>(0,i)[1];
		}
	}

	topLeft.x=xValues[1];
	topLeft.y=minY;
	bottomRight.x=xValues[2];
	bottomRight.y=maxY;
}

