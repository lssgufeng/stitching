#include "Matching.h"

void Matching::GetMatchesBrief(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
		this->extractor=new cv::BriefDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
}

void Matching::GetMatchesSurf(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
		int64 tick=cv::getTickCount();
		this->extractor=new cv::SurfDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
		printf("GetMatchesSurf Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());		
}

void Matching::GetMatchesSurfThread(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
		int64 tick=cv::getTickCount();
		this->extractor=new cv::SurfDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		//this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
		threadDataKnn matchData1={this->descriptors1,this->descriptors2,matches1};
		threadDataKnn matchData2={this->descriptors2,this->descriptors1,matches2};

		HANDLE hThreads[2];
		
		hThreads[0]=(HANDLE)_beginthread(knnMatch,0,(void*)&matchData1);
		hThreads[1]=(HANDLE)_beginthread(knnMatch,0,(void*)&matchData2);
		WaitForMultipleObjects(2,hThreads,TRUE,INFINITE);
		printf("GetMatchesSurf Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());		
}

void Matching::GetMatchesSurf_Flann(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches1,std::vector<cv::DMatch>& matches2){
		int64 tick=cv::getTickCount();
		this->extractor=new cv::SurfDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching_Flann(this->descriptors1,this->descriptors2,matches1,matches2);
		printf("Descriptor count=%d GetMatchesSurf_Flann Took %f Seconds, ",this->descriptors1.rows,(cv::getTickCount()-tick)/cv::getTickFrequency());
}
void Matching::GetMatchesSurf_FlannThread(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches1,std::vector<cv::DMatch>& matches2){
		int64 tick=cv::getTickCount();
		this->extractor=new cv::SurfDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		threadDataFlann matchData1={this->descriptors1,this->descriptors2,matches1};
		threadDataFlann matchData2={this->descriptors2,this->descriptors1,matches2};

		HANDLE hThreads[2];
		
		hThreads[0]=(HANDLE)_beginthread(flannMatch,0,(void*)&matchData1);
		hThreads[1]=(HANDLE)_beginthread(flannMatch,0,(void*)&matchData2);

		WaitForMultipleObjects(2,hThreads,TRUE,INFINITE);
		printf("GetMatchesSurf Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
}


void Matching::GetMatchesFreak(cv::Mat& image1, cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches1,std::vector<cv::DMatch>& matches2){
		//this->extractor=new cv::FREAK();		
		cv::FREAK extractor;
		extractor.compute(image1,keyPoints1,this->descriptors1);
		extractor.compute(image2,keyPoints2,this->descriptors2);
		this->performMatching_Freak(this->descriptors1,this->descriptors2,matches1,matches2);
}
void Matching::GetMatchesFreakThread(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<cv::DMatch>& matches1,std::vector<cv::DMatch>& matches2){
		int64 tick=cv::getTickCount();
		cv::FREAK extractor;
		extractor.compute(image1,keyPoints1,this->descriptors1);
		extractor.compute(image2,keyPoints2,this->descriptors2);
		//this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
		threadDataFreak matchData1={this->descriptors1,this->descriptors2,matches1};
		threadDataFreak matchData2={this->descriptors2,this->descriptors1,matches2};

		HANDLE hThreads[2];
		
		hThreads[0]=(HANDLE)_beginthread(hammingMatch,0,(void*)&matchData1);
		hThreads[1]=(HANDLE)_beginthread(hammingMatch,0,(void*)&matchData2);
		WaitForMultipleObjects(2,hThreads,TRUE,INFINITE);
		printf("GetMatchesFreak Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());		
}



void Matching::GetMatchesSift(cv::Mat& image1,cv::Mat& image2,
	std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
		this->extractor=new cv::SiftDescriptorExtractor();
		this->extractor->compute(image1,keyPoints1,this->descriptors1);
		this->extractor->compute(image2,keyPoints2,this->descriptors2);
		this->performMatching(this->descriptors1,this->descriptors2,matches1,matches2);
}

void Matching::performMatching(cv::Mat descriptors1, cv::Mat descriptors2,
	std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2){
		int64 tick=cv::getTickCount();
		cv::BruteForceMatcher<cv::L2<float>> matcher;
		matcher.knnMatch(descriptors1,descriptors2,matches1,2);
		matcher.knnMatch(descriptors2,descriptors1,matches2,2);
		printf("PerformMatching Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
}

void Matching::performMatching_Flann(cv::Mat descriptors1, cv::Mat descriptors2,
	std::vector<cv::DMatch>& matches1, std::vector<cv::DMatch>& matches2){		
		int64 tick=cv::getTickCount();
		cv::FlannBasedMatcher matcher;
		matcher.match(descriptors1,descriptors2,matches1);
		matcher.match(descriptors2,descriptors1,matches2);
		printf("Flann Matching Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());


}
void Matching::performMatching_Freak(cv::Mat descriptors1, cv::Mat descriptors2,
	std::vector<cv::DMatch>& matches1, std::vector<cv::DMatch>& matches2){
		cv::BruteForceMatcher<cv::Hamming> matcher;
		matcher.match(descriptors2, descriptors1,matches2);
		matcher.match(descriptors1,descriptors2,matches1);
}

void knnMatch(void* threadArg){
	int64 tick=cv::getTickCount();
	struct threadDataKnn* matchData;
	matchData=(struct threadDataKnn*)threadArg;
	cv::BruteForceMatcher<cv::L2<float>> matcher;
    matcher.knnMatch(matchData->descriptors1,matchData->descriptors2,matchData->matches,2);	
	printf("knn match took %f seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
}

void flannMatch(void* threadArg){
	int64 tick=cv::getTickCount();
	struct threadDataFlann* matchData;
	matchData=(struct threadDataFlann*)threadArg;
	cv::FlannBasedMatcher matcher;
    matcher.match(matchData->descriptors1,matchData->descriptors2,matchData->matches);	
	printf("knn match took %f seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
}
void hammingMatch(void* threadArg){
	struct threadDataFreak* matchData;
	matchData=(struct threadDataFreak*)threadArg;
	cv::BruteForceMatcher<cv::Hamming> matcher;
	matcher.match(matchData->descriptors1,matchData->descriptors2,matchData->matches);
}


int Matching::RatioTest(std::vector<std::vector<cv::DMatch>>& matches,double threshold){
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

void Matching::SymmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
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
			printf("Symmetry Test Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
}

void Matching::SymmetryTest_Flann(const std::vector<cv::DMatch>& matches1,
	const std::vector<cv::DMatch>& matches2,
	std::vector<cv::DMatch>& symMatches){
		int64 tick=cv::getTickCount();
		for(std::vector<cv::DMatch>::const_iterator matchIterator1=matches1.begin();
			matchIterator1!=matches1.end();++matchIterator1){
				for(std::vector<cv::DMatch>::const_iterator matchIterator2=matches2.begin();
					matchIterator2!=matches2.end();++matchIterator2){
						bool condition=(*matchIterator1).queryIdx==(*matchIterator2).trainIdx
							&&(*matchIterator1).trainIdx==(*matchIterator2).queryIdx;
							if(condition){
								symMatches.push_back(cv::DMatch((*matchIterator1).queryIdx,
									(*matchIterator1).trainIdx,(*matchIterator1).distance));								
								break;
							}
				}
		}
		printf("Symmetry matches removed=%d points",matches1.size()-symMatches.size());
		printf("Symmetry Test Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
}

void Matching::SymmetryTest_Freak(const std::vector<cv::DMatch>& matches1,
	const std::vector<cv::DMatch>& matches2,
	std::vector<cv::DMatch>& symMatches){
		int64 tick=cv::getTickCount();
		for(std::vector<cv::DMatch>::const_iterator matchIterator1=matches1.begin();
			matchIterator1!=matches1.end();++matchIterator1){
				for(std::vector<cv::DMatch>::const_iterator matchIterator2=matches2.begin();
					matchIterator2!=matches2.end();++matchIterator2){
						bool condition=(*matchIterator1).queryIdx==(*matchIterator2).trainIdx
							&&(*matchIterator1).trainIdx==(*matchIterator2).queryIdx;
							if(condition){
								symMatches.push_back(cv::DMatch((*matchIterator1).queryIdx,
									(*matchIterator1).trainIdx,(*matchIterator1).distance));								
								break;
							}
				}
		}
		//symMatches=matches1;
		printf("Symmetry matches removed=%d points",matches1.size()-symMatches.size());
		printf("Symmetry Test Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
}


cv::Mat Matching::RansacTest(const std::vector<cv::DMatch>& goodMatches,
		const std::vector<cv::KeyPoint>& keyPoints1,
		const std::vector<cv::KeyPoint>& keyPoints2,
		double distance, double confidence,
		std::vector<cv::DMatch>& resultMatches){
			//Convert keypoints into Point2f
			std::vector<cv::Point2f> points1,points2;
			this->GetFloatPoints(keyPoints1,keyPoints2,goodMatches,points1,points2);
			//Compute Fundamental Matrix
			std::vector<uchar> inliers(points1.size(),0);
			cv::Mat fundamental=cv::findFundamentalMat(cv::Mat(points1),
				cv::Mat(points2),
				inliers,
				CV_FM_RANSAC, //RANSAC Method
				distance, //distance to epipolar line
				confidence); //confidence probability
			
			//Get the inlier points
			std::vector<uchar>::const_iterator iteratorInlier=
				inliers.begin();
			std::vector<cv::DMatch>::const_iterator iteratorMatch=
				goodMatches.begin();
			for(;iteratorInlier!=inliers.end();++iteratorInlier,++iteratorMatch){
				if(*iteratorInlier){
					resultMatches.push_back(*iteratorMatch);
				}
			}

			//Refinement of Fundamental Matrix from the accepted matches
			points1.clear();
			points2.clear();
			this->GetFloatPoints(keyPoints1,keyPoints2,resultMatches,points1,points2);


			return fundamental;
}

cv::Mat Matching::GetHomography(const std::vector<cv::DMatch>& goodMatches,
	std::vector<cv::KeyPoint>& keyPoints1,
	std::vector<cv::KeyPoint>& keyPoints2,
	std::vector<uchar>& inliers){
		int64 tick =cv::getTickCount();
		std::vector<cv::Point2f> points1,points2;
		//convert to floating point for homography		
		this->GetFloatPoints(keyPoints1,keyPoints2,goodMatches,points1,points2);
		cv::Mat homography= cv::findHomography(
			cv::Mat(points1), //Corresponding 
			cv::Mat(points2), //matching points
			inliers,          //inliers
			CV_RANSAC,        //using RANSAC method
			3);        		  //maximum pixel distance		
		printf("GetHomography Took %f Seconds",(cv::getTickCount()-tick)/cv::getTickFrequency());
		return homography;
}

void Matching::DrawMatches(cv::Mat image1,std::vector<cv::KeyPoint> keyPoints1,
		cv::Mat image2,std::vector<cv::KeyPoint> keyPoints2,
		std::vector<cv::DMatch> matches, cv::Mat& outputImage){
			cv::drawMatches(image1,keyPoints1,image2,keyPoints2,matches,outputImage);
}

void Matching::DrawInliers(std::vector<cv::Point2f> points,std::vector<uchar>& inliers, cv::Mat image,cv::Mat& outImage){
	outImage=image.clone();
	printf("Inliers=%d",inliers.size());
	std::vector<cv::Point2f>::const_iterator iteratorMatches=points.begin();
	std::vector<uchar>::const_iterator iteratorInliers=inliers.begin();
	while(iteratorInliers!=inliers.end()){
		if(*iteratorInliers){
			cv::circle(outImage,*iteratorMatches,3,cv::Scalar(-1));
		}
		++iteratorInliers;++iteratorMatches;
	}
}


void Matching::GetFloatPoints(const std::vector<cv::KeyPoint>& keyPoints1,const std::vector<cv::KeyPoint>& keyPoints2,
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








