#include<stdio.h>
#include<time.h>
#include<iostream>
#include "opencv2\core\core.hpp"
#include "opencv2\calib3d\calib3d.hpp"
#include "opencv2\contrib\contrib.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "MyLog.h"


//This is overall stitching test
class StitchingTest{
	cv::Mat image1;
	cv::Mat image2;
	int distanceThreshold;
	int level;
	//Defines the boundry point
	//@Value is the co-ordinate value
	//@Index is image index
	struct Boundry{
		int Value;
		int Index;
	};

	enum ImageInfo{
		NONE,
		FLOAT,
		BASE		
	};

	enum BlendDirection{
		NODIR,
		DIR1,
		DIR2,
		BOTH
	};
	struct Neighbor{
		ImageInfo Left;
		ImageInfo Top;
		ImageInfo Bottom;
		ImageInfo Right;
	};

	struct BlendMask{
		cv::Mat_<float> Left;
		cv::Mat_<float> Top;
		cv::Mat_<float> Right;
		cv::Mat_<float> Bottom;
	};
	

public:
	StitchingTest(){
		char* path1="images/l_rot_8.jpg";
		char* path2="images/r.jpg";
		image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
		image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
		distanceThreshold=1;
		level=4;
		performOverallStitch(2);
		/*Neighbor neighbor;
		neighbor.Top=ImageInfo::FLOAT;
		neighbor.Left=ImageInfo::BASE;
		neighbor.Right=ImageInfo::BASE;
		neighbor.Bottom=ImageInfo::FLOAT;

		createBlendMask(150,150,neighbor);*/
		getchar();
	}
	~StitchingTest(){
	}

	void performOverallStitch(int direction){
		MyLog log;
		char* resultFile="result/stitching.txt";
		cv::Mat homography=calculateHomography();
		cv::Mat warpedImage;
		cv::Point topLeft, bottomRight;
		rotateImage(image1,homography,warpedImage,topLeft,bottomRight);
		Boundry left, top, right, bottom;
		if(topLeft.x<0){
			left.Index=0;
			left.Value=topLeft.x;
		}else{
			left.Index=1;
			left.Value=0;
		}
		
		if(topLeft.y<0){
			top.Index=0;
			top.Value=topLeft.y;
		}else{
			top.Index=1;
			top.Value=0;
		}
		
		if(bottomRight.x>image2.cols){
			right.Index=0;
			right.Value=bottomRight.x;
		}else{
			right.Index=1;
			right.Value=image2.cols;
		}

		if(bottomRight.y>image2.rows){
			bottom.Index=0;
			bottom.Value=bottomRight.y;
		}else{
			bottom.Index=1;
			bottom.Value=image2.rows;
		}


		cv::Mat stitchedImage(bottom.Value-top.Value+1,right.Value-left.Value+1,CV_8U);

		cv::Rect floatRegion, baseRegion, commonStitchedRegion, commonBaseRegion, commonFloatRegion;
		
		floatRegion.width=warpedImage.cols;
		floatRegion.height=warpedImage.rows;
		baseRegion.width=image2.cols;
		baseRegion.height=image2.rows;
		
		if(left.Index==0){
			floatRegion.x=0;
			baseRegion.x=abs(left.Value);
			if(top.Index==0){				
				floatRegion.y=0;
				baseRegion.y=abs(top.Value);	
				//Common Region
				int commonWidth=cv::min(warpedImage.cols-abs(topLeft.x),image2.cols);
				int commonHeight=cv::min(warpedImage.rows-abs(topLeft.y),image2.rows);
				commonFloatRegion.width=commonBaseRegion.width=commonStitchedRegion.width=commonWidth;
				commonFloatRegion.height=commonBaseRegion.height=commonStitchedRegion.height=commonHeight;
				
				commonFloatRegion.x=abs(topLeft.x);
				commonFloatRegion.y=abs(topLeft.y);

				commonBaseRegion.x=0;
				commonBaseRegion.y=0;

				commonStitchedRegion.x=abs(topLeft.x);
				commonStitchedRegion.y=abs(topLeft.y);
			}else{				
				floatRegion.y=topLeft.y;
				baseRegion.y=0;

			//Common Region
			int commonWidth=cv::min(warpedImage.cols-abs(topLeft.x),image2.cols);
			int commonHeight=cv::min(warpedImage.rows,image2.rows-topLeft.y);

			commonFloatRegion.width=commonBaseRegion.width=commonStitchedRegion.width=commonWidth;
			commonFloatRegion.height=commonBaseRegion.height=commonStitchedRegion.height=commonHeight;
			
			commonFloatRegion.x=abs(topLeft.x);
			commonFloatRegion.y=0;

			commonBaseRegion.x=0;
			commonBaseRegion.y=abs(topLeft.y);

			commonStitchedRegion.x=abs(topLeft.x);
			commonStitchedRegion.y=topLeft.y;
		}
		}else{
			floatRegion.x=topLeft.x;
			baseRegion.x=0;
			if(top.Index==0){				
				floatRegion.y=0;
				baseRegion.y=abs(top.Value);
				//Common Region
				int commonWidth=cv::min(warpedImage.cols,image2.cols-topLeft.x);
				int commonHeight=cv::min(warpedImage.rows-abs(topLeft.y),image2.rows);

				commonFloatRegion.width=commonBaseRegion.width=commonStitchedRegion.width=commonWidth;
				commonFloatRegion.height=commonBaseRegion.height=commonStitchedRegion.height=commonHeight;

				commonFloatRegion.x=0;
				commonFloatRegion.y=abs(topLeft.y);

				commonBaseRegion.x=topLeft.x;
				commonBaseRegion.y=0;
				
				commonStitchedRegion.x=topLeft.x;
				commonStitchedRegion.y=abs(topLeft.y);

		}else{			
			floatRegion.y=topLeft.y;
			baseRegion.y=0;

			//Common Region
			int commonWidth=cv::min(warpedImage.cols,image2.cols-topLeft.x);
			int commonHeight=cv::min(warpedImage.rows,image2.rows-topLeft.y);

			commonFloatRegion.width=commonBaseRegion.width=commonStitchedRegion.width=commonWidth;
			commonFloatRegion.height=commonBaseRegion.height=commonStitchedRegion.height=commonHeight;
			
			commonFloatRegion.x=0;
			commonFloatRegion.y=0;

			commonBaseRegion.x=topLeft.x;
			commonBaseRegion.y=topLeft.y;

			commonStitchedRegion.x=topLeft.x;
			commonStitchedRegion.y=topLeft.y;
		}
	}
		warpedImage.copyTo(stitchedImage(floatRegion));
		image2.copyTo(stitchedImage(baseRegion));
		//cv::imwrite("result/blending/raw-composite-image.png",stitchedImage);


		//cv::imwrite("result/blending/o_common_base.png",image2(commonBaseRegion));
		//cv::imwrite("result/blending/o_common_float.png",warpedImage(commonFloatRegion));


		cv::Mat blendedRegion;//=cv::Mat(commonFloatRegion.height,commonFloatRegion.width,CV_8U);

		Neighbor neighbor;
		if(direction==0){
			neighbor.Left=ImageInfo::FLOAT; neighbor.Right=ImageInfo::BASE;	
			neighbor.Top=ImageInfo::NONE;neighbor.Bottom=ImageInfo::NONE;
		}else if(direction==1){
			neighbor.Top =ImageInfo::FLOAT;neighbor.Bottom=ImageInfo::BASE;
			neighbor.Left=ImageInfo::NONE;neighbor.Right=ImageInfo::NONE;
		}else{
			neighbor.Left=left.Index==0?ImageInfo::FLOAT:ImageInfo::BASE;
			neighbor.Top=top.Index==0?ImageInfo::FLOAT:ImageInfo::BASE;
			neighbor.Right=right.Index==0?ImageInfo::FLOAT:ImageInfo::BASE;
			neighbor.Bottom=bottom.Index==0?ImageInfo::FLOAT:ImageInfo::BASE;
		}

		float alphaTime=performAlphaBlend(warpedImage(commonFloatRegion),image2(commonBaseRegion),neighbor,blendedRegion);
		/*cv::Mat reference;
		stitchedImage(commonStitchedRegion).copyTo(reference);
		cv::Mat alphaDiff;
		cv::absdiff(reference,blendedRegion,alphaDiff);
		cv::imwrite("result/blending/Diff_Alpha.png",alphaDiff);*/
		blendedRegion.copyTo(stitchedImage(commonStitchedRegion));
		cv::imshow("StitchedImage",stitchedImage);
		cv::waitKey(0);
		/*float laplacianTime=performLaplacianBlend(warpedImage(commonFloatRegion),image2(commonBaseRegion),blendedRegion);
		cv::Mat pyramidDifference;
		cv::absdiff(reference,blendedRegion,pyramidDifference);
		cv::imwrite("result/blending/Diff_Pyramid.png",pyramidDifference);
		blendedRegion.copyTo(stitchedImage(commonStitchedRegion));
		cv::imwrite("result/blending/StitchedImage(Laplacian).png",stitchedImage);	

		log.Write(resultFile,"Alpha Blending Took %f seconds",alphaTime);
		log.Write(resultFile,"Pyramid Blending Took %f seconds",laplacianTime);

		float alphaError=0.0,laplaceError=0.0;
		
		for(int i=0;i<alphaDiff.rows;i++){
			for(int j=0;j<alphaDiff.cols;j++){
				alphaError+=alphaDiff.at<uchar>(i,j)/255.0; 
				laplaceError+=pyramidDifference.at<uchar>(i,j)/255.0;
			}
		}

		log.Write(resultFile,"Alpha Blending Inaccuracy=%f ",alphaError);
		log.Write(resultFile,"Pyramid Bleidng Inaccuracy=%f",laplaceError);*/
	}

	void rotateImage(const cv::Mat image,cv::Mat homography,cv::Mat& outputImage,
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

		getCorners(corners,topLeft,bottomRight); 

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
			cv::INTER_NEAREST,0,0);

	
}

	cv::Mat calculateHomography(){		
		std::vector<cv::KeyPoint> keyPoints1;
		std::vector<cv::KeyPoint> keyPoints2;
		cv::Mat descriptor1, descriptor2;
		
		cv::Ptr<cv::FeatureDetector> detector=new cv::SurfFeatureDetector(20);
		detector->detect(image1,keyPoints1);
		//detector=new cv::SurfFeatureDetector(100);
		//detector=new cv::SurfFeatureDetector(400);
		detector->detect(image2, keyPoints2);

		cv::Ptr<cv::DescriptorExtractor> extractor=new cv::SurfDescriptorExtractor();
		extractor->compute(image1,keyPoints1,descriptor1);
		extractor->compute(image2,keyPoints2,descriptor2);

		/*
		std::vector<cv::DMatch> flannSymmetryMatches;		
		cv::FlannBasedMatcher flannBasedMatcher;		
		std::vector<std::vector<cv::DMatch>> flannMatches1, flannMatches2;

		flannBasedMatcher.knnMatch(descriptor1,descriptor2,flannMatches1,2);      
		flannBasedMatcher.knnMatch(descriptor2, descriptor1,flannMatches2,2);

		RatioTest(flannMatches1,0.8);
		RatioTest(flannMatches2,0.8);
		SymmetryTest(flannMatches1,flannMatches2,flannSymmetryMatches);
		*/

		std::vector<cv::DMatch> bruteForceSymmetryMatches;		
		cv::FlannBasedMatcher bruteForceBasedMatcher;		
		std::vector<std::vector<cv::DMatch>> bruteForceMatches1, bruteForceMatches2;

		bruteForceBasedMatcher.knnMatch(descriptor1,descriptor2,bruteForceMatches1,2);      
		bruteForceBasedMatcher.knnMatch(descriptor2, descriptor1,bruteForceMatches2,2);

		RatioTest(bruteForceMatches1,0.8);
		RatioTest(bruteForceMatches2,0.8);


		SymmetryTest(bruteForceMatches1,bruteForceMatches2,bruteForceSymmetryMatches);	
		
		
		std::vector<cv::Point2f> points1,points2;
		std::vector<uchar> inliers;
		GetFloatPoints(keyPoints1,keyPoints2,bruteForceSymmetryMatches,points1,points2);
		
		cv::Mat homography= cv::findHomography(
			cv::Mat(points1), //Corresponding 
			cv::Mat(points2), //matching points
			inliers,          //inliers
			CV_RANSAC,        //using RANSAC method
			distanceThreshold);//maximum pixel distance	

		return homography;
	}

	void getCorners(const cv::Mat corners,cv::Point& topLeft,cv::Point& bottomRight){
	topLeft.x=cv::min(cv::min((double)corners.at<cv::Vec2f>(0,0)[0],(double)corners.at<cv::Vec2f>(0,1)[0]),
		cv::min((double)corners.at<cv::Vec2f>(0,2)[0],(double)corners.at<cv::Vec2f>(0,3)[0]));
	topLeft.y=cv::min(cv::min((double)corners.at<cv::Vec2f>(0,0)[1],(double)corners.at<cv::Vec2f>(0,1)[1]),
		cv::min((double)corners.at<cv::Vec2f>(0,2)[1],(double)corners.at<cv::Vec2f>(0,3)[1]));

	bottomRight.x=cv::max(cv::max((double)corners.at<cv::Vec2f>(0,0)[0],(double)corners.at<cv::Vec2f>(0,1)[0]),
		cv::max((double)corners.at<cv::Vec2f>(0,2)[0],(double)corners.at<cv::Vec2f>(0,3)[0]));
	bottomRight.y=cv::max(cv::max((double)corners.at<cv::Vec2f>(0,0)[1],(double)corners.at<cv::Vec2f>(0,1)[1]),
		cv::max((double)corners.at<cv::Vec2f>(0,2)[1],(double)corners.at<cv::Vec2f>(0,3)[1]));
}


	int SymmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
		const std::vector<std::vector<cv::DMatch>>& matches2,
		std::vector<cv::DMatch>& symMatches){
			int64 tick=cv::getTickCount();
			//check for image1->imag2 matches
			for(std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator1=matches1.begin();
				matchIterator1!=matches1.end();++matchIterator1){
					if(matchIterator1->size()<2) continue;
					//check for image2->image1 matches
					for(std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator2=matches2.begin();
						matchIterator2!=matches2.end();++matchIterator2){
							if(matchIterator2->size()<2)continue;
							//Match Symmetry Test
							bool condition=(*matchIterator1)[0].queryIdx==(*matchIterator2)[0].trainIdx
								&&(*matchIterator1)[0].trainIdx==(*matchIterator2)[0].queryIdx;
							if(condition){
								symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx,
									(*matchIterator1)[0].trainIdx,(*matchIterator1)[0].distance));								
								break;
							}						
					}
			}
			//printf("Symmetry matches removed=%d points",matches1.size()-symMatches.size());
			//printf("Symmetry Test Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());			
			return symMatches.size();
	}

//int SymmetryTest_Flann(const std::vector<cv::DMatch>& matches1,
//	const std::vector<cv::DMatch>& matches2,
//	std::vector<cv::DMatch>& symMatches){
//		int64 tick=cv::getTickCount();
//		for(std::vector<cv::DMatch>::const_iterator matchIterator1=matches1.begin();
//			matchIterator1!=matches1.end();++matchIterator1){
//				for(std::vector<cv::DMatch>::const_iterator matchIterator2=matches2.begin();
//					matchIterator2!=matches2.end();++matchIterator2){
//						bool condition=(*matchIterator1).queryIdx==(*matchIterator2).trainIdx
//							&&(*matchIterator1).trainIdx==(*matchIterator2).queryIdx;
//							if(condition){
//								symMatches.push_back(cv::DMatch((*matchIterator1).queryIdx,
//									(*matchIterator1).trainIdx,(*matchIterator1).distance));								
//								break;
//							}
//				}
//		}
//		printf("Flann Symmetry matches removed=%d points",matches1.size()-symMatches.size());
//		printf("Flann Symmetry Test Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
//}

int RatioTest(std::vector<std::vector<cv::DMatch>>& matches,double threshold){
	int removed=0;
	for(std::vector<std::vector<cv::DMatch>>::iterator matchIterator=matches.begin();
		matchIterator!=matches.end();++matchIterator){
			if(matchIterator->size()>1){
				if((*matchIterator)[0].distance/(*matchIterator)[1].distance>threshold){
					matchIterator->clear();					
					removed++;
				}
			}else{
				matchIterator->clear();
				removed++;
			}
	}
	return removed;
}

void GetFloatPoints(const std::vector<cv::KeyPoint>& keyPoints1,const std::vector<cv::KeyPoint>& keyPoints2,
	const std::vector<cv::DMatch>& matches, 
	std::vector<cv::Point2f>& points1,
	std::vector<cv::Point2f>& points2){
		for(std::vector<cv::DMatch>::const_iterator iterator=matches.begin();
			iterator!=matches.end();
			++iterator){
				float x=keyPoints1[iterator->queryIdx].pt.x;
				float y=keyPoints1[iterator->queryIdx].pt.y;
				points1.push_back(cv::Point2f(x,y));
				x=keyPoints2[iterator->trainIdx].pt.x;
				y=keyPoints2[iterator->trainIdx].pt.y;
				points2.push_back(cv::Point2f(x,y));
	}
}



float performAlphaBlend(const cv::Mat& image1, cv::Mat& image2,Neighbor neighbor,cv::Mat& outputImage){
	int64 tick=cv::getTickCount();
	cv::Mat_<float> blendedImage[5];
	for(int i=0;i<5;i++){
		blendedImage[i].create(image1.rows,image1.cols);
		//cv::imshow("BlendedImage",blendedImage[i]);
		//cv::waitKey(0);
	}
	  


	cv::Mat_<float> image1F,image2F;
	image1.convertTo(image1F,CV_32F,1.0/255.0);
	image2.convertTo(image2F,CV_32F,1.0/255.0);
	
	//BlendMask masks=createHorizontalBlendask(image1.rows,image1.cols);
	BlendMask masks=createBlendMask(image1.rows,image1.cols,neighbor);
	cv::Mat_<float> blendMask[4];
	blendMask[0]=masks.Left;
	blendMask[1]=masks.Top;
	blendMask[2]=masks.Right;
	blendMask[3]=masks.Bottom;

	cv::Mat temp;
	for(int i=0;i<4;i++){
		char* name=(char*)malloc(100*sizeof(char));
		sprintf(name,"result/blending/blending-mask-%d.png",i);
		blendMask[i].convertTo(temp,CV_8U,255);
		cv::imwrite(name,temp);
		/*cv::imshow("Mask",blendMask[i]);
		cv::waitKey(0);*/
	}


	if(neighbor.Left==ImageInfo::FLOAT){
		blendedImage[0]=blendMask[0].mul(image1F);
	}else if(neighbor.Left==ImageInfo::BASE){
		blendedImage[0]=blendMask[0].mul(image2F);
	}

	if(neighbor.Top==ImageInfo::FLOAT){
		blendedImage[1]=blendMask[1].mul(image1F);
	}else if(neighbor.Top==ImageInfo::BASE){
		blendedImage[1]=blendMask[1].mul(image2F);
	}


	if(neighbor.Right==ImageInfo::FLOAT){
		blendedImage[2]=blendMask[2].mul(image1F);
	}else if(neighbor.Right==ImageInfo::BASE){
		blendedImage[2]=blendMask[2].mul(image2F);
	}

	if(neighbor.Bottom==ImageInfo::FLOAT){
		blendedImage[3]=blendMask[3].mul(image1F);
	}else if(neighbor.Bottom==ImageInfo::BASE){
		blendedImage[3]=blendMask[3].mul(image2F);
	}

	blendedImage[4]=blendedImage[0]+blendedImage[1]+blendedImage[2]+blendedImage[3];

	for(int i=0;i<5;i++){
		cv::imshow("Blended",blendedImage[i]);
		cv::waitKey(0);
	}
	
	//cout<<blendedImage[4];
	blendedImage[4].convertTo(outputImage,CV_8U,255);
	
	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();

	cv::imwrite("result/blending/alpha_blend.png",outputImage);
	return elapsedTime;
}

#pragma region Pyramid Blending
float performLaplacianBlend(const cv::Mat& top, const cv::Mat& bottom, cv::Mat& blendedImage){	
	int64 tick=cv::getTickCount();
	
	cv::Mat_<cv::Vec3f> t,b; 
	cv::Vector<cv::Mat_<cv::Vec3f>> topLapPyr, bottomLapPyr, resultPyr;

	//Smallest Images
	cv::Mat_<cv::Vec3f> topSmallestLevel, bottomSmallestLevel,resultSmallestLevel;
	//Mask Gaussian Pyramid
	cv::Vector<cv::Mat_<cv::Vec3f>> maskGaussianPyramid;
	

	top.convertTo(t,CV_32F,1.0/255.0);
	bottom.convertTo(b,CV_32F,1.0/255.0);
	

	cv::cvtColor(t,t,CV_GRAY2BGR);
	cv::cvtColor(b,b,CV_GRAY2BGR);


	generateLaplacianPyramid(t,topLapPyr,topSmallestLevel);
	generateLaplacianPyramid(b,bottomLapPyr,bottomSmallestLevel);


	//Blend Mask
	BlendMask masks=createHorizontalBlendMask(t.rows,t.cols);

	cv::Mat_<float> blendMask=masks.Left;

	/*for(int i=0;i<t.cols;i++){
		for(int j=0;j<t.rows;j++){
			if(i==0 && j==0){
				blendMask.at<float>(j,i)=1.0;
				continue;
			}
			int shortX=std::min(i,(t.cols-i));
			int shortY=std::min(j,(t.rows-j));
			blendMask.at<float>(j,i)=1.0-(float)shortX/(shortX+shortY);
		}
	}*/


	generateGaussianPyramid(blendMask, topLapPyr, topSmallestLevel, maskGaussianPyramid);
	
	/*cv::Mat temp;
	cv::cvtColor(topSmallestLevel,temp,CV_RGB2GRAY);
	temp.convertTo(temp,CV_8U,255.0);*/
	//cv::imwrite("result/blending/top_smallest.png",temp);

	/*cv::cvtColor(bottomSmallestLevel,temp,CV_RGB2GRAY);
	temp.convertTo(temp,CV_8U,255.0);*/
	//cv::imwrite("result/blending/bottom_smallest.png",temp);


	//for(int i=0;i<level;i++){
	//	char name[100];
		//sprintf(name,"result/blending/pyramids/gaussian/gaussian_pyramids_%d.png",i);
		//cv::Mat temp1,temp2;
		//cv::cvtColor(maskGaussianPyramid[i],temp1,CV_RGB2GRAY);
		//temp1.convertTo(temp2,CV_8U,255.0);
		//cv::imwrite(name,temp2);

		//sprintf(name,"result/blending/pyramids/laplacian/image1/lap_pyramids_%d.png",i);
	//	cv::cvtColor(topLapPyr[i],temp1,CV_RGB2GRAY);
	//	temp1.convertTo(temp2,CV_8U,255.0);
	//	cv::imwrite(name,temp2);

		

		//sprintf(name,"result/blending/pyramids/laplacian/image2/lap_pyramids_%d.png",i);
		//cv::cvtColor(bottomLapPyr[i],temp1,CV_RGB2GRAY);
		//temp1.convertTo(temp2,CV_8U,255.0);
		//cv::imwrite(name,temp2);

	//}
	//generateGaussianPyramid(blendMask, bottomLapPyr, bottomSmallestLevel, bottomMaskGaussianPyramid);
	blendLapPyrs(topLapPyr,bottomLapPyr,topSmallestLevel,bottomSmallestLevel,blendMask,maskGaussianPyramid,resultSmallestLevel,resultPyr);
	cv::Mat blendedImage_32=reconstructImage(resultPyr,resultSmallestLevel);
	cv::cvtColor(blendedImage_32,blendedImage_32,CV_BGR2GRAY);	
	blendedImage_32.convertTo(blendedImage,CV_8U,255);

	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();
	cv::imwrite("result/blending/lap_pyr_blend.png",blendedImage);
	return elapsedTime;
	
}





void generateLaplacianPyramid(const cv::Mat_<cv::Vec3f>& image,
	cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr,
	cv::Mat_<cv::Vec3f>& smallestLevel){		
			lapPyr.clear();
			cv::Mat_<cv::Vec3f> currentImage=image.clone();
			for(int i=0;i<level;i++){
				cv::Mat_<cv::Vec3f> down, up;
				cv::pyrDown(currentImage,down);
				cv::pyrUp(down, up,currentImage.size());
				cv::Mat_<cv::Vec3f> lap;
				cv::absdiff(currentImage,up,lap);
				lapPyr.push_back(lap);
				currentImage=down;				
			}
			currentImage.copyTo(smallestLevel);			
}

void generateGaussianPyramid(cv::Mat& blendMask,cv::Vector<cv::Mat_<cv::Vec3f>> lapPyr,cv::Mat smallestLevel,
	cv::Vector<cv::Mat_<cv::Vec3f>>& maskGaussianPyramid){
		assert(lapPyr.size()>0);
		
		maskGaussianPyramid.clear();
		
		cv::Mat currentImage;
		
		cv::cvtColor(blendMask,currentImage,CV_GRAY2BGR);
		maskGaussianPyramid.push_back(currentImage);
		
		currentImage=blendMask;
		
		for(int i=1;i<level+1;i++){
			cv::Mat _down;
			if(i<lapPyr.size()){
				cv::pyrDown(currentImage,_down,lapPyr[i].size());
			}else{
				cv::pyrDown(currentImage,_down,smallestLevel.size());
			}
			/*char message[100];
			sprintf(message,"Gaussian Mask: Level %d",i);*/
			cv::Mat down;
			cv::cvtColor(_down,down,CV_GRAY2BGR);
			maskGaussianPyramid.push_back(down);
			currentImage=_down;
		}
}

void blendLapPyrs(cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr1,
	cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr2, 
	cv::Mat_<cv::Vec3f>& smallestLevel1,cv::Mat_<cv::Vec3f>& smallestLevel2, 
	cv::Mat_<float>& blendMask,
	cv::Vector<cv::Mat_<cv::Vec3f>> maskGaussianPyramid,
	cv::Mat& resultSmallestLevel,cv::Vector<cv::Mat_<cv::Vec3f>>& resultPyr){
		/*printf("Smallest Level1 Size:row=%d, column=%d\t Mask gauss. Size row=%d col=%d",
			smallestLevel1.rows,smallestLevel1.cols,maskGaussianPyramid.back().rows,maskGaussianPyramid.back().cols); 
		printf("Smallest Level2 Size:row=%d, column=%d\t Mask gauss. Size row=%d col=%d",
			smallestLevel2.rows,smallestLevel2.cols,maskGaussianPyramid.back().rows,maskGaussianPyramid.back().cols); */
	

		resultSmallestLevel=smallestLevel1.mul(maskGaussianPyramid.back())+smallestLevel2.mul(cv::Scalar(1.0,1.0,1.0)-maskGaussianPyramid.back());
			
		for(int i=0;i<level;i++){
			cv::Mat A=lapPyr1[i].mul(maskGaussianPyramid[i]);
			cv::Mat antiMask=cv::Scalar(1.0,1.0,1.0)-maskGaussianPyramid[i];
			cv::Mat B=lapPyr2[i].mul(antiMask);
			cv::Mat_<cv::Vec3f> result=A+B;
			resultPyr.push_back(result);		
		}
}

cv::Mat reconstructImage(cv::Vector<cv::Mat_<cv::Vec3f>> resultPyr,cv::Mat resultSmallestLevel){
	cv::Mat currentImage=resultSmallestLevel;
	for(int i=level-1; i>=0;i--){
		cv::Mat up;
		cv::pyrUp(currentImage,up,resultPyr[i].size());
		currentImage=up+resultPyr[i];
	}
	return currentImage;
}

#pragma endregion Pyramid Blending Ends

BlendMask createHorizontalBlendMask(int rows, int cols){
	BlendMask masks;
	masks.Left=masks.Top=masks.Right=masks.Bottom=cv::Mat_<float>(rows,cols,0.0);
	cv::Mat_<float> blendMask[4];
	blendMask[0]=cv::Mat_<float>(rows,cols,0.0);
	blendMask[1]=cv::Mat_<float>(rows,cols,0.0);
	blendMask[2]=cv::Mat_<float>(rows,cols,0.0);
	blendMask[3]=cv::Mat_<float>(rows,cols,0.0);

	for(int j=0;j<cols;j++){
		float value1=(float)(cols-j)/cols;
		float value2=(float)j/cols;
		//printf("value1=%f\tvalue2=%f\n",value1,value2);
		for(int i=0;i<rows;i++){
			blendMask[0].at<float>(i,j)=value1;
			blendMask[2].at<float>(i,j)=value2;
		}
	}
	masks.Left=blendMask[0];
	masks.Right=blendMask[2];
	cv::Mat tmpImage;
	blendMask[0].convertTo(tmpImage,CV_8U,255);	
	cv::imwrite("result/blending/LeftMask.png",tmpImage);
	blendMask[2].convertTo(tmpImage,CV_8U,255);
	cv::imwrite("result/blending/RightMask.png",tmpImage);
	return masks;
}

BlendMask createBlendMask(int rows, int cols, Neighbor neighbor){
	MyLog log;
	char* resultFile="result/blending/blending.txt";
	log.Write(resultFile,"-----------------------------------------------\n--------------------------------");

	int maxInt=numeric_limits<int>::max();

	BlendMask masks;
	masks.Left=masks.Top=masks.Right=masks.Bottom=cv::Mat_<float>(rows,cols,0.0);

	cv::Mat_<float> blendMask[4];
	blendMask[0]=cv::Mat_<float>(rows,cols,0.0);
	blendMask[1]=cv::Mat_<float>(rows,cols,0.0);
	blendMask[2]=cv::Mat_<float>(rows,cols,0.0);
	blendMask[3]=cv::Mat_<float>(rows,cols,0.0);
	
	float value[4]={0.0,0.0,0.0,0.0};
	cv::Mat_<float> totalMask;
	int d[4],dtemp[4], index[4];
	float cum=0.0;
	for(int row=0;row<rows;row++){
		for(int col=0;col<cols;col++){
			/*d[0]=dtemp[0]=neighbor.Left==ImageInfo::NONE?numeric_limits<int>::max():col;
			d[1]=dtemp[1]=neighbor.Top==ImageInfo::NONE?numeric_limits<int>::max():row;
			d[2]=dtemp[2]=neighbor.Right==ImageInfo::NONE?numeric_limits<int>::max():cols-col;
			d[3]=dtemp[3]=neighbor.Bottom==ImageInfo::NONE?numeric_limits<int>::max():rows-row;*/
			int denominator[4];
			if(row==col && row==0)
			{
				value[0]=0.5;
				value[1]=0.5;
				value[2]=0.0;
				value[3]=0.0;
			}else{
				int minX=cv::min(col,cols-col);
				int minY=cv::min(row,rows-row);
				float valueX=(1.0-(float)minX/(minX+minY));
				float valueY=(1-(float)minY/(minX+minY));
				value[0]=valueX*(1-(float)col/cols);
				value[2]=valueX*(float)col/cols;
				value[1]=valueY*(1-(float)row/rows);
				value[3]=valueY*(float)row/rows;
				cum=value[0]+value[1]+value[2]+value[3];
			}
			blendMask[0].at<float>(row,col)=value[0];
			blendMask[1].at<float>(row,col)=value[1];
			blendMask[2].at<float>(row,col)=value[2];
			blendMask[3].at<float>(row,col)=value[3];

			//log.Write(resultFile,"(%d,%d)value[0]=%f value[1]=%f value[2]=%f value[3]=%f , sum=%f\n",row,col,value[0],value[1],value[2],value[3],cum);
		}
	}
			
	masks.Left=blendMask[0];masks.Top=blendMask[1];masks.Right=blendMask[2];masks.Bottom=blendMask[3];

	return masks;
}
};



