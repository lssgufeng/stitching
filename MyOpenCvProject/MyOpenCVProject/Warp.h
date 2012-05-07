/***
* @class Warp
* @description It handles all image warping related operations. Generally it 
*              does its operations 
* @author Krishna (krishna444@gmail.com)
***/
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"

class Warp{
private:
public:
	//Get the warped points of the given points using the 
	//homography supplied. This function can be used to transfrom 
	//from single point to an array of points. e.g. we can get the newly
	//transformed poi
	void GetWarpPoints(cv::Mat& homography,
		cv::Mat& srcPts,
		cv::Mat& dstPts);
	//Gets the warped point for a single point using the homography
	void GetWarpPoint(cv::Mat& homography,
		cv::Point& point,
		cv::Point& dstPoint);

	
};