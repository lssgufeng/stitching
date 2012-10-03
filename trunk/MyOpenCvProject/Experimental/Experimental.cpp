// Experimental.cpp : Defines the entry point for the console application.
//
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

std::vector<cv::Point> ExtractHarrisFeatures(char*,char*);
std::vector<cv::KeyPoint> ExtractSIFTFeatures(char*,char*);
std::vector<cv::KeyPoint> ExtractSURFFeatures(char*,char*);
void MatchFeatures(double threshold);
void EvaluateNN(double threshold);
int SymmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
	const std::vector<std::vector<cv::DMatch>>& matches2,
		std::vector<cv::DMatch>& symMatches);
//void SymmetryTest_Flann(const std::vector<cv::DMatch>& matches1,
//	const std::vector<cv::DMatch>& matches2,
//	std::vector<cv::DMatch>& symMatches);
void AccurateMatches(double threshold);
int RatioTest(std::vector<std::vector<cv::DMatch>>& matches,double threshold);
cv::Mat HomographyTest();
void GetFloatPoints(const std::vector<cv::KeyPoint>& keyPoints1,const std::vector<cv::KeyPoint>& keyPoints2,
	const std::vector<cv::DMatch>& matches, 
	std::vector<cv::Point2f>& points1,
	std::vector<cv::Point2f>& points2);

void performAlphaBlendX(const cv::Mat& leftImage,const cv::Mat& rightImage,cv::Mat& outputImage);
void performAlphaBlendY(const cv::Mat& topImage,const cv::Mat& bottomImage,cv::Mat& outputImage);

void performLaplacianBlend(const cv::Mat& top, const cv::Mat& bottom, cv::Mat& outputImage);
void performAlphaBlend(const cv::Mat& image1, cv::Mat& image2,cv::Mat& outputImage);
void generateLaplacianPyramid(const cv::Mat& image,
	cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr,
	cv::Mat& smallestLevel);
void generateGaussianPyramid(cv::Mat& blendMask,cv::Vector<cv::Mat_<cv::Vec3f>> lapPyr,cv::Mat smallestLevel,
	cv::Vector<cv::Mat_<cv::Vec3f>>& maskGaussianPyramid);
void blendLapPyrs(cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr1,
	cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr2, 
	cv::Mat& smallestLevel1,cv::Mat& smallestLevel2, 
	cv::Mat_<float>& blendMask,
	cv::Vector<cv::Mat_<cv::Vec3f>> maskGaussianPyramid,
	cv::Mat& resultSmallestLevel,cv::Vector<cv::Mat_<cv::Vec3f>>& resultPyr);
cv::Mat reconstructImage(cv::Vector<cv::Mat_<cv::Vec3f>> resultPyr,cv::Mat resultSmallestLevel);
void BlendingTest();

char files[][100]={"l.jpg","l_br.jpg","l_rot_8.jpg","l_large.jpg","l_br_rot.jpg","l_large_br.jpg","l_large_br_rot.jpg","l_noise.jpg"};
bool heading=true;
int level=10;


int main(void)
{
	////Testing for feature extraction
	//for(int i=0; i<8; i++){
	//	ExtractHarrisFeatures(files[i],"result/harris/result.txt");
	//	ExtractSIFTFeatures(files[i],"result/SIFT/result.txt");
	//	ExtractSURFFeatures(files[i],"result/SURF/result.txt");
	//}

	/*for(int i=1000; i>100; i-=25)
		MatchFeatures(i);*/
	/*for(int i=1000;i>100;i-=25){
		printf(".");
		EvaluateNN(i);
	}*/
	/*EvaluateNN(100);*/
	//AccurateMatches(100);		

	//HomographyTest();
	BlendingTest();
	/*char* path1="images/blending/l.png";
	char* path2="images/blending/r.png";
	cv::Mat image1=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	image1.convertTo(image1,CV_8U,1/255.0);
	cv::imwrite("images/blending/r_8.png",image1);*/

}

#pragma region Other
std::vector<cv::Point> ExtractHarrisFeatures(char* imageFile,char* resultFile){	
	MyLog log;
	time_t curr;
	time(&curr);	
	log.Write(resultFile,ctime(&curr));
	char* imageFullPath=new char[200];
	char* saveFullPath=new char[200];

	std::vector<cv::Point> basePoints;
	strcpy(imageFullPath,"images/");
	imageFullPath=strcat(imageFullPath,imageFile);
	strcpy(saveFullPath,"result/harris/");
	saveFullPath=strcat(saveFullPath,imageFile);

	cv::Mat image1=cv::imread(imageFullPath,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);	
	//Harris Detector
	int neighbourhood=3;
	int aperture=11;
	double k=0.04;
	double maxStrength=4;
	double threshold=0.000001;
	int nonMaxSize=12;

	//LOG
	log.Write(resultFile,"Image Supplied:%s",imageFullPath);


	cv::Mat cornerStrength;
	//harrisResult.create(image1.rows,image1.cols,CV_32FC1);
	int64 tick=cv::getTickCount();
	cv::cornerHarris(image1,cornerStrength, neighbourhood,aperture,k,cv::BORDER_DEFAULT);
	//internal threshold computation
	double minStrength;
	cv::minMaxLoc(cornerStrength,&minStrength,&maxStrength);
	//local maxima detection
	cv::Mat dilated;
	cv::dilate(cornerStrength,dilated,cv::Mat());
	cv::Mat localMax;
	cv::compare(cornerStrength,dilated,localMax,cv::CMP_EQ);

	double qualityLevel=0.04;
	threshold= qualityLevel*maxStrength;
	cv::Mat cornerTh;
	cv::threshold(cornerStrength,cornerTh,
		threshold,255,cv::THRESH_BINARY);
	cv::Mat cornerMap;
	cornerTh.convertTo(cornerMap,CV_8U);
	//non-maxima suppression
	cv::bitwise_and(cornerMap,localMax,cornerMap);

	std::vector<cv::Point> points;
	for(int y=0;y<cornerMap.rows;y++){
			 const uchar* rowPtr=cornerMap.ptr<uchar>(y);
			 for(int x=0;x<cornerMap.cols;x++){
				 //if it is a feature point
				 if(rowPtr[x]){
					 points.push_back(cv::Point(x,y));
				 }
			 }
		 }

	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();
	
	//LOG 
	log.Write(resultFile,"Elapsed Time=%f seconds",elapsedTime);
	log.Write(resultFile,"Got %d points",points.size());

	std::vector<cv::Point>::const_iterator it=points.begin();
		 //for all corners
		 while(it!=points.end()){
			 //draw a circle
			 cv::circle(image1,*it,3, cv::Scalar(0),2);
			 ++it;
		 }		 

		 cv::imwrite(saveFullPath,image1);
		 return points;
}

std::vector<cv::KeyPoint> ExtractSIFTFeatures(char* imageFile, char* resultFile){
	MyLog log;
	time_t curr;
	time(&curr);	
	log.Write(resultFile,ctime(&curr));
	char* imageFullPath=new char[200];
	char* saveFullPath=new char[200];

	std::vector<cv::Point> basePoints;
	strcpy(imageFullPath,"images/");
	imageFullPath=strcat(imageFullPath,imageFile);
	strcpy(saveFullPath,"result/SIFT/");
	saveFullPath=strcat(saveFullPath,imageFile);
	cv::Mat image1=cv::imread(imageFullPath,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	//LOG
	log.Write(resultFile,"Image Supplied:%s",imageFullPath);

	double threshold=0.04;
	double linesThreshold=10;
	std::vector<cv::KeyPoint> keyPoints;
	int64 tick=cv::getTickCount();
	cv::Ptr<cv::FeatureDetector> detector=new cv::SiftFeatureDetector(threshold,linesThreshold);
	detector->detect(image1,keyPoints);
	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();

	log.Write(resultFile,"Elapsed Time=%f Seconds.",elapsedTime);
	log.Write(resultFile,"Got %d key points",keyPoints.size());
	cv::Mat tmpImage;
	cv::drawKeypoints(image1,keyPoints,tmpImage);
	cv::imwrite(saveFullPath,tmpImage);	
	return keyPoints;
}

std::vector<cv::KeyPoint> ExtractSURFFeatures(char* imageFile, char* resultFile){	
	MyLog log;
	time_t curr;
	time(&curr);	
	log.Write(resultFile,ctime(&curr));
	char* imageFullPath=new char[200];
	char* saveFullPath=new char[200];

	std::vector<cv::Point> basePoints;
	strcpy(imageFullPath,"images/");
	imageFullPath=strcat(imageFullPath,imageFile);
	strcpy(saveFullPath,"result/SURF/");
	saveFullPath=strcat(saveFullPath,imageFile);
	cv::Mat image1=cv::imread(imageFullPath,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	//LOG
	log.Write(resultFile,"Image Supplied:%s",imageFullPath);

	double threshold=700;
	std::vector<cv::KeyPoint> keyPoints;
	int64 tick=cv::getTickCount();
	cv::Ptr<cv::FeatureDetector> detector=new cv::SurfFeatureDetector(threshold);
	detector->detect(image1,keyPoints);
	float elapsedTime=(cv::getTickCount()-tick)/cv::getTickFrequency();

	log.Write(resultFile,"Elapsed Time=%f Seconds.",elapsedTime);
	log.Write(resultFile,"Got %d key points",keyPoints.size());

	cv::Mat tmpImage;
	cv::drawKeypoints(image1,keyPoints,tmpImage);
	cv::imwrite(saveFullPath,tmpImage);
	return keyPoints;
}

void MatchFeatures(double threshold){
	MyLog log;
	/*time_t curr;
	time(&curr);	*/
	char* resultFile="result/matching/matching.txt";
	/*log.Write(resultFile,ctime(&curr));*/


	char* path1="images/l.jpg";
	char* path2="images/r.jpg";
	double SIFTTime=0.0;
	double SURFTime=0.0;

	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	
	std::vector<cv::KeyPoint> keyPoints1;
	std::vector<cv::KeyPoint> keyPoints2;
	cv::Mat descriptor1, descriptor2;
	std::vector<std::vector<cv::DMatch>> matches;
		
	cv::Ptr<cv::FeatureDetector> detector=new cv::SurfFeatureDetector(threshold);
	detector->detect(image1,keyPoints1);
	detector=new cv::SurfFeatureDetector(400);
	detector->detect(image2, keyPoints2);
	cv::BruteForceMatcher<cv::L2<float>> matcher;

	/*log.Write(resultFile,"Key Points1 Count:%d",keyPoints1.size());
	log.Write(resultFile,"Key Points2 Count: %d",keyPoints2.size());*/

	//START FROM SIFT
	int64 tick=cv::getTickCount();
	cv::Ptr<cv::DescriptorExtractor> extractor=new cv::SiftDescriptorExtractor();
	extractor->compute(image1,keyPoints1,descriptor1);
	extractor->compute(image2,keyPoints2,descriptor2);	
	
	matcher.knnMatch(descriptor1,descriptor2,matches,1);
	SIFTTime=(cv::getTickCount()-tick)/cv::getTickFrequency();


	//SURF	
	tick=cv::getTickCount();
	extractor=new cv::SurfDescriptorExtractor();
	extractor->compute(image1,keyPoints1,descriptor1);
	extractor->compute(image2,keyPoints2,descriptor2);	
	matcher.knnMatch(descriptor1,descriptor2,matches,1);
	SURFTime=(cv::getTickCount()-tick)/cv::getTickFrequency();

	//LOG
	log.Write(resultFile,"%d \t %d \t %f \t %f",keyPoints1.size(), keyPoints2.size(),SIFTTime,SURFTime);  
}


void EvaluateNN(double threshold){
	MyLog log;
	/*time_t curr;
	time(&curr);	*/
	char* resultFile="result/matching/knn_ann.txt";
	/*log.Write(resultFile,ctime(&curr));*/


	char* path1="images/l.jpg";
	char* path2="images/r.jpg";
	double SIFTTime=0.0;
	double SURFTime=0.0;

	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	
	//cv::medianBlur(image1,image1,3);
	//cv::medianBlur(image2,image2,3);

	
	std::vector<cv::KeyPoint> keyPoints1;
	std::vector<cv::KeyPoint> keyPoints2;
	cv::Mat descriptor1, descriptor2;
	std::vector<std::vector<cv::DMatch>> bruteForceMatches;
	std::vector<cv::DMatch> flannMatches;
		
	cv::Ptr<cv::FeatureDetector> detector=new cv::SurfFeatureDetector(threshold);
	detector->detect(image1,keyPoints1);
	detector=new cv::SurfFeatureDetector(threshold);
	//detector=new cv::SurfFeatureDetector(400);
	detector->detect(image2, keyPoints2);
	cv::BruteForceMatcher<cv::L2<float>> bruteForceMatcher;
	cv::FlannBasedMatcher flannBasedMatcher;

	cv::Ptr<cv::DescriptorExtractor> extractor=new cv::SurfDescriptorExtractor();
	extractor->compute(image1,keyPoints1,descriptor1);
	extractor->compute(image2,keyPoints2,descriptor2);
    
	double kNNTime=0.0;
	double flannTime=0.0;

	int64 tick=cv::getTickCount();
	bruteForceMatcher.knnMatch(descriptor1,descriptor2,bruteForceMatches,1);
	kNNTime=(cv::getTickCount()-tick)/cv::getTickFrequency();
	tick=cv::getTickCount();
	flannBasedMatcher.knnMatch(descriptor1,descriptor2,bruteForceMatches,1);
	flannTime=(cv::getTickCount()-tick)/cv::getTickFrequency();

	log.Write(resultFile,"%d\t%d\t%f\t%f",keyPoints1.size(),keyPoints2.size(),kNNTime,flannTime);

	//Further Test
	cv::Mat outputImage;	
	cv::drawMatches(image1,keyPoints1,image2,keyPoints2,bruteForceMatches,outputImage);
	cv::imwrite("result/matching/Knn_Match.png",outputImage);	
	
	std::vector<cv::DMatch> symmetryMatches;

	cv::drawMatches(image1,keyPoints1,image2,keyPoints2,flannMatches,outputImage);
	cv::imwrite("result/matching/flann_Match.png",outputImage);

	//AccurateMatches(image1,image2,keyPoints1,keyPoints2,descriptor1,descriptor2);
}

void AccurateMatches(double threshold){
	MyLog log;
	/*time_t curr;
	time(&curr);	*/
	char* resultFile="result/matching/accurate_matching.txt";
	/*log.Write(resultFile,ctime(&curr));*/

	char* path1="images/l.jpg";
	char* path2="images/r.jpg";
	double SIFTTime=0.0;
	double SURFTime=0.0;

	cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);

	std::vector<cv::KeyPoint> keyPoints1;
	std::vector<cv::KeyPoint> keyPoints2;
	cv::Mat descriptor1, descriptor2;

	cv::Ptr<cv::FeatureDetector> detector=new cv::SurfFeatureDetector(threshold);
	detector->detect(image1,keyPoints1);
	detector=new cv::SurfFeatureDetector(threshold);
	//detector=new cv::SurfFeatureDetector(400);
	detector->detect(image2, keyPoints2);

	cv::Ptr<cv::DescriptorExtractor> extractor=new cv::SurfDescriptorExtractor();
	extractor->compute(image1,keyPoints1,descriptor1);
	extractor->compute(image2,keyPoints2,descriptor2);

	std::vector<cv::DMatch> bruteSymmetryMatches,flannSymmetryMatches;
	cv::BruteForceMatcher<cv::L2<float>> bruteForceMatcher;
	cv::FlannBasedMatcher flannBasedMatcher;
	std::vector<std::vector<cv::DMatch>> bruteForceMatches1,bruteForceMatches2;	
	std::vector<std::vector<cv::DMatch>> flannMatches1, flannMatches2;
	cv::Mat outputImage;

	bruteForceMatcher.knnMatch(descriptor1,descriptor2,bruteForceMatches1,2);
	bruteForceMatcher.knnMatch(descriptor2,descriptor1,bruteForceMatches2,2);

	log.Write(resultFile,"kNN matches: %d",bruteForceMatches1.size());

	int count=RatioTest(bruteForceMatches1,0.8);
	RatioTest(bruteForceMatches2,0.8);

	log.Write(resultFile,"After ratio test: removed=%d points",count);


	cv::drawMatches(image1,keyPoints1,image2,keyPoints2,bruteForceMatches1,outputImage);
	cv::imwrite("result/matching/knn_ratio.png",outputImage);

	SymmetryTest(bruteForceMatches1,bruteForceMatches2,bruteSymmetryMatches);	
	log.Write(resultFile,"After symmetry test: got %d points",bruteSymmetryMatches.size());
		
	cv::drawMatches(image1,keyPoints1,image2,keyPoints2,bruteSymmetryMatches,outputImage);
	cv::imwrite("result/matching/knn_symmetry.png",outputImage);
	

	flannBasedMatcher.knnMatch(descriptor1,descriptor2,flannMatches1,2);      
	flannBasedMatcher.knnMatch(descriptor2, descriptor1,flannMatches2,2);

	cv::drawMatches(image1,keyPoints1,image2, keyPoints2,flannMatches1,outputImage);
	cv::imwrite("result/matching/ann_match.png",outputImage);

	log.Write(resultFile,"ANN matches:%d",flannMatches1.size());

	count=RatioTest(flannMatches1,0.8);
	RatioTest(flannMatches2,0.8);

	log.Write(resultFile,"After ratio test: removed %d points",count);

	std::vector<cv::DMatch> flannMatches1Best;
	
	std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator=flannMatches1.begin();
	for(;matchIterator!=flannMatches1.end();matchIterator++){
		if((*matchIterator).size()==2)
			flannMatches1Best.push_back((*matchIterator)[0]);
	}

	cv::drawMatches(image1,keyPoints1,image2,keyPoints2,flannMatches1Best,outputImage);
	cv::imwrite("result/matching/flann_ratio.png",outputImage);

	SymmetryTest(flannMatches1,flannMatches2,flannSymmetryMatches);
	cv::drawMatches(image1,keyPoints1,image2,keyPoints2,flannSymmetryMatches,outputImage);
	log.Write(resultFile,"After symmetry matches, got %d points",flannSymmetryMatches.size());

	cv::imwrite("result/matching/flann_symmetry.png",outputImage);
	getchar();
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


cv::Mat HomographyTest(){
		MyLog log;
		char* resultFile="result/homography/homography.txt";
		char* path1="images/l.jpg";
		char* path2="images/r.jpg";

		cv::Mat image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
		
		std::vector<cv::KeyPoint> keyPoints1;
		std::vector<cv::KeyPoint> keyPoints2;
		cv::Mat descriptor1, descriptor2;
		
		cv::Ptr<cv::FeatureDetector> detector=new cv::SurfFeatureDetector(100);
		detector->detect(image1,keyPoints1);
		detector=new cv::SurfFeatureDetector(100);
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

		if(heading){
			log.Write(resultFile,"Threshold\tMatches\tInliers");
			heading=false;
		}
		
		
		std::vector<cv::Point2f> points1,points2;
		std::vector<uchar> inliers;
		GetFloatPoints(keyPoints1,keyPoints2,bruteForceSymmetryMatches,points1,points2);

		for(int distanceThreshold=0;distanceThreshold<10;distanceThreshold++){
			cv::Mat homography= cv::findHomography(
				cv::Mat(points1), //Corresponding 
				cv::Mat(points2), //matching points
				inliers,          //inliers
				CV_RANSAC,        //using RANSAC method
				distanceThreshold);//maximum pixel distance
			
			int inliersCount=0;
			for(std::vector<uchar>::const_iterator iterator=inliers.begin();
				iterator!=inliers.end();++iterator){
					if(*iterator){
						inliersCount++;
					}
			}

			log.Write(resultFile,"%d\t%d\t%d",distanceThreshold,bruteForceSymmetryMatches.size(),inliersCount);

			std::vector<cv::DMatch>::const_iterator symmetryIterator=bruteForceSymmetryMatches.begin();
			std::vector<uchar>::const_iterator inliersIterator=inliers.begin();		
			
			std::vector<cv::DMatch> inlierMatches;
			for(;symmetryIterator!=bruteForceSymmetryMatches.end();symmetryIterator++,inliersIterator++){
				if(*inliersIterator){			
					inlierMatches.push_back(*symmetryIterator);
				}
			}
			
			if(distanceThreshold==0){
			cv::Mat outputImage;
			cv::drawMatches(image1,keyPoints1,image2,keyPoints2,inlierMatches,outputImage);			
			cv::imwrite("result/homography/inliers.png",outputImage);
			}
		}
		cv::Mat result;
		return result;
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

#pragma endregion Other Operations

void BlendingTest(){
	MyLog log;
	char* resultFile="result/homography/homography.txt";
	char* path1="images/blending/r.png";
	char* path2="images/blending/r_br.png";
	cv::Mat image1, image2,blendImageAlpha, blendImageLapPyr;
	image1=cv::imread(path1,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	image2=cv::imread(path2,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_GRAYSCALE);
	performAlphaBlend(image1,image2,blendImageAlpha);
	performLaplacianBlend(image1,image2,blendImageLapPyr);
	cv::imwrite("result/blending/diff1.png",10*(blendImageAlpha-blendImageLapPyr));
	cv::imwrite("result/blending/diff2.png",10*(blendImageLapPyr-blendImageAlpha));
	//cout<<(blendImageAlpha-blendImageLapPyr);
	
}

void performAlphaBlendX(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage){
	double alpha=1,beta=0;
	for(int i=0;i<image1.cols;i++){
		beta=(double)i/(image1.cols-1);
		alpha=1-beta;
		cv::addWeighted(image1.col(i),alpha,image2.col(i),beta,0,outputImage.col(i));
	}
}

void performAlphaBlendY(const cv::Mat& image1,const cv::Mat& image2,cv::Mat& outputImage){
	double alpha=1,beta=0;
	for(int i=0;i<image1.rows;i++){
		beta=(double)i/(image1.rows-1);
		alpha=1-beta;
		cv::addWeighted(image1.row(i),alpha,image2.row(i),beta,0,outputImage.row(i));
	}
}

void performAlphaBlend(const cv::Mat& image1, cv::Mat& image2,cv::Mat& outputImage){
	double alpha=1.0, beta=0.0;
	outputImage.create(image1.rows,image1.cols,image1.type());
	outputImage.at<uchar>(0,0)=image1.at<uchar>(0,0);
	for(int i=1;i<image1.rows;i++){
		for(int j=1;j<image1.cols;j++){
			int shortY=std::min(i,(image1.rows-i));
			int shortX=std::min(j,(image1.cols-j));
			beta=(double)shortX/(shortX+shortY);
			alpha=1.0-beta;
			//printf("%d",alpha);
			outputImage.at<uchar>(i,j)=alpha*image1.at<uchar>(i,j)+beta*image2.at<uchar>(i,j);
		}
	}
	cv::imwrite("result/blending/alpha_blend.png",outputImage);
}

void performLaplacianBlend(const cv::Mat& top, const cv::Mat& bottom, cv::Mat& blendedImage){	
	cv::Mat_<cv::Vec3f> t,b; 
	cv::Vector<cv::Mat_<cv::Vec3f>> topLapPyr, bottomLapPyr, resultPyr;

	//Smallest Images
	cv::Mat topSmallestLevel, bottomSmallestLevel,resultSmallestLevel;
	//Mask Gaussian Pyramid
	cv::Vector<cv::Mat_<cv::Vec3f>> maskGaussianPyramid;
	

	top.convertTo(t,CV_32F,1.0/255.0);
	bottom.convertTo(b,CV_32F,1.0/255.0);
	

	cv::cvtColor(t,t,CV_GRAY2BGR);
	cv::cvtColor(b,b,CV_GRAY2BGR);


	generateLaplacianPyramid(t,topLapPyr,topSmallestLevel);
	generateLaplacianPyramid(b,bottomLapPyr,bottomSmallestLevel);


	//Blend Mask
	cv::Mat_<float> blendMask(t.rows,t.cols,0.0);

	for(int i=1;i<t.cols;i++){
		for(int j=1;j<t.rows;j++){
			int shortX=std::min(i,(t.cols-i));
			int shortY=std::min(j,(t.rows-j));
			blendMask.at<float>(j,i)=1.0-(float)shortX/(shortX+shortY);
		}
	}


	generateGaussianPyramid(blendMask, topLapPyr, topSmallestLevel, maskGaussianPyramid);
	//generateGaussianPyramid(blendMask, bottomLapPyr, bottomSmallestLevel, bottomMaskGaussianPyramid);
	blendLapPyrs(topLapPyr,bottomLapPyr,topSmallestLevel,bottomSmallestLevel,blendMask,maskGaussianPyramid,resultSmallestLevel,resultPyr);
	cv::Mat blendedImage_32=reconstructImage(resultPyr,resultSmallestLevel);
	cv::cvtColor(blendedImage_32,blendedImage_32,CV_BGR2GRAY);
	
	
	blendedImage_32.convertTo(blendedImage,CV_8U,255);
	cv::imwrite("result/blending/lap_pyr_blend.png",blendedImage);
	
}

void generateLaplacianPyramid(const cv::Mat& image,
	cv::Vector<cv::Mat_<cv::Vec3f>>& lapPyr,
	cv::Mat& smallestLevel){
			lapPyr.clear();
			cv::Mat currentImage=image.clone();
			for(int i=0;i<level;i++){
				cv::Mat down, up;
				cv::pyrDown(currentImage,down);
				cv::pyrUp(down, up,currentImage.size());
				cv::Mat_<cv::Vec3f> lap=currentImage-up;
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
	cv::Mat& smallestLevel1,cv::Mat& smallestLevel2, 
	cv::Mat_<float>& blendMask,
	cv::Vector<cv::Mat_<cv::Vec3f>> maskGaussianPyramid,
	cv::Mat& resultSmallestLevel,cv::Vector<cv::Mat_<cv::Vec3f>>& resultPyr){
		printf("Smallest Level1 Size:row=%d, column=%d\t Mask gauss. Size row=%d col=%d",
			smallestLevel1.rows,smallestLevel1.cols,maskGaussianPyramid.back().rows,maskGaussianPyramid.back().cols); 
		printf("Smallest Level2 Size:row=%d, column=%d\t Mask gauss. Size row=%d col=%d",
			smallestLevel2.rows,smallestLevel2.cols,maskGaussianPyramid.back().rows,maskGaussianPyramid.back().cols); 
	

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

