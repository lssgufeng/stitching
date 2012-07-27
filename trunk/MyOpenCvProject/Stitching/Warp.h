/***
* @class Warp
* @description It handles all image warping related operations. Generally it 
*              does its operations using homography matrix
* @author Krishna (krishna444@gmail.com)
***/
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"


class Warp{
private:
	
public:
	//Constructor
	Warp();
	//Destructor
	~Warp();
	//Get the warped points of the given points using the 
	//homography supplied. This function can be used to transfrom 
	//from single point to an array of points. e.g. we can get the newly
	//transformed poi
	void GetWarpPoints(cv::Mat& srcPts,
		cv::Mat& dstPts,
		cv::Mat& homography);
	//Gets the warped point for a single point using the homography
	//Its slower because it calls GetWarpPoints function to work
	void GetWarpPoint(cv::Mat& homography,
		cv::Point& point,
		cv::Point& dstPoint);
	//It is another method to transform point. It is copied from
    //http://stackoverflow.com/questions/4279008/specify-an-origin-to-warpperspective-function-in-opencv-2-x
	//it is faster. 
	void TransformPoint(const cv::Point pointToTransform,
		cv::Point& outputPoint,
		const cv::Mat& homography);
     
	//It tranforms the corners of the image
	//@corners[] arrays of corners
	//@outputPoints the transformed corners
	//@homography everyybody understands
	void Warp::TransformCorners(const cv::Point* corners,
	cv::Point* outputCorners,
	const cv::Mat homography);

	//It is the main method for image transformation. We use the result image
	//to join to base image to get stitched image. The ROIs of this image are
	//copied to the resultant image.
	void RotateImage(const cv::Mat image,cv::Mat homography,cv::Mat& outputImage,cv::Point& topLeft,cv::Point& bottomRight);
	//This rotates the image and crops vertically
	void RotateImage_Xcrop(cv::Mat image,cv::Mat homography,bool crop, cv::Mat& outputImage, cv::Point& topLeft, cv::Point& bottomRight);
	//This rotates the image and crops horizontally
	void RotateImage_Ycrop(cv::Mat image,cv::Mat homography,bool crop, cv::Mat& outputImage, cv::Point& topLeft, cv::Point& bottomRight);

	/***
	* Tests the transformation of the image with the supplied parameters
	* For testing only. Actually we use homography matrix and will be faster.
	* @image the image to transform
	* @angle rotation angle(in degrees)
	* @xTrans x translation
	* @yTrans y translation
	***/
	void TestTransformation(cv::Mat& image,
		double angle,
		double xTrans, 
		double yTrans);
	//Gets the homography defined by specified angle 
	//and translation(x-, and y-directions)
	//Note: angle is always degrees, i cant understand radians :)
	void GetCustomHomography(double angle, 
		double xTrans, 
		double yTrans,
		cv::Mat& homography);
    //Custom Warp Perspective which does rotation along the origin
	void WarpPerspective( const cv::Mat& src, cv::Mat& dst, const cv::Mat& M0, cv::Size dsize,
        int flags, int borderType, const cv::Scalar& borderValue, CvPoint origin );

	//It gets the extrement corners i.e. top left and bottom right 
	//The resulting area is the maximum possible area
	void GetCorners(const cv::Point corners[],cv::Point& topLeft, cv::Point& bottomRight);
	void GetCorners(const cv::Mat corners,cv::Point& topLeft,cv::Point& bottomRight);
	//We get the corners of x croped image.
	//Returns the shifted X
	void GetCorners_Xcrop(const cv:: Point corners[],bool crop, cv::Point& topLeft, cv::Point& bottomRight);
	void GetCorners_Xcrop(const cv::Mat corners,bool crop,cv::Point& topLeft,cv::Point& bottomRight);
	//Get the corners of y croped image
	//Returns the shifted Y used for stitching
	void GetCorners_Ycrop(const cv::Point corners[],bool crop, cv::Point& topLeft, cv::Point& bottomRight);
	void GetCorners_Ycrop(const cv::Mat corners,bool crop,cv::Point& topLeft,cv::Point& bottomRight);
};