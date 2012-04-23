/***
* @ class Matching
* @ description matching process is carried out after we detect the key points. 
* @ author Krishna (krishna444@gmail.com)
**/
#include "opencv2\calib3d\calib3d.hpp"
#include "opencv2\contrib\contrib.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
class Matching {
private:
	//private members
	cv::Ptr<cv::DescriptorExtractor> extractor;
	cv::Mat descriptors1,descriptors2;
	void performMatching(cv::Mat descriptors1,cv::Mat descriptors2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);
public:
	//Get the matches using the Brief Descriptors
	void GetMatchesBrief(cv::Mat& image1,cv::Mat& image2,
		std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);	
	//Get the matches using Surf Descriptors
	void GetMatchesSurf(cv::Mat& image1,cv::Mat& image2,
		std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);
	//Get the matches using Sift Descriptors
	void GetMatchesSift(cv::Mat& image1,cv::Mat& image2,
		std::vector<cv::KeyPoint>& keyPoints1,std::vector<cv::KeyPoint>& keyPoints2,
		std::vector<std::vector<cv::DMatch>>& matches1,std::vector<std::vector<cv::DMatch>>& matches2);

	//Performs the ratio test of the matches found
	int RatioTest(std::vector<std::vector<cv::DMatch>>& matches,double threshold);
	//Perform the symmetry test. Returns the matches selected
	void SymmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
		const std::vector<std::vector<cv::DMatch>>& matches2,
		std::vector<cv::DMatch>& symMatches);
	//Perform RANSAC Test to get the best matched points. It returns fundamental matrix
	cv::Mat RansacTest(const std::vector<cv::DMatch>& matches,
		const std::vector<cv::KeyPoint>& keyPoints1,
		const std::vector<cv::KeyPoint>& keyPoints2,
		double distance, double confidence,
		std::vector<cv::DMatch>& resultMatches);
};