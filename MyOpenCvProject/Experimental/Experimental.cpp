// Experimental.cpp : Defines the entry point for the console application.
//
#include<stdio.h>
#include<time.h>
#include "opencv2\core\core.hpp"
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




char files[][100]={"l.jpg","l_br.jpg","l_rot_8.jpg","l_large.jpg","l_br_rot.jpg","l_large_br.jpg","l_large_br_rot.jpg","l_noise.jpg"};

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
	AccurateMatches(100);		
}

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

	log.Write(resultFile,"ANN matches:%d",flannMatches1.size());

	count=RatioTest(flannMatches1,0.8);
	RatioTest(flannMatches2,0.8);

	log.Write(resultFile,"After ratio test: removed %d points",flannMatches1.size());

	cv::drawMatches(image1,keyPoints1,image2,keyPoints2,flannMatches1,outputImage);
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



