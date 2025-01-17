#include "opencv2/opencv.hpp"
 
using namespace cv;
 

/*
@author Roy says:
I want to share a small piece of code to do Laplacian Blending using OpenCV. It�s one of the most basic and canonical methods of image blending, and is a must exercise for any computer graphics student.

Well basically it�s a matter of creating two Laplacian pyramids of both images, and a Gaussian pyramid of the mask.
Then we blend the pyramids into one, and collapse the resulting pyramid into the blended image.
@url:http://www.morethantechnical.com/2011/11/13/just-a-simple-laplacian-pyramid-blender-using-opencv-wcode/
*/
class LaplacianBlending {
private:
    Mat_<Vec3f> left;
    Mat_<Vec3f> right;
    Mat_<float> blendMask;
 
    vector<Mat_<Vec3f> > leftLapPyr,rightLapPyr,resultLapPyr;
    Mat leftSmallestLevel, rightSmallestLevel, resultSmallestLevel;
    vector<Mat_<Vec3f> > maskGaussianPyramid; //masks are 3-channels for easier multiplication with RGB
 
    int levels;
 
    void buildPyramids(){
        buildLaplacianPyramid(left,leftLapPyr,leftSmallestLevel);
        buildLaplacianPyramid(right,rightLapPyr,rightSmallestLevel);
        buildGaussianPyramid();
    }
 
    void buildGaussianPyramid() {
        assert(leftLapPyr.size()>0);
 
        maskGaussianPyramid.clear();
        Mat currentImg;
        cvtColor(blendMask, currentImg, CV_GRAY2BGR);
        maskGaussianPyramid.push_back(currentImg); //highest level
 
        currentImg = blendMask;
        for (int l=1; l<levels+1; l++) {
            Mat _down;
            if (leftLapPyr.size() > l) {
                pyrDown(currentImg, _down, leftLapPyr[l].size());
            } else {
                pyrDown(currentImg, _down, leftSmallestLevel.size()); //smallest level
            }
 
            Mat down;
            cvtColor(_down, down, CV_GRAY2BGR);
            maskGaussianPyramid.push_back(down);
            currentImg = _down;
        }
    }
 
    void buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& smallestLevel) {
        lapPyr.clear();
		Mat currentImg = img.clone();
        for (int l=0; l<levels; l++) {
            Mat down,up;
            pyrDown(currentImg, down);
            pyrUp(down, up, currentImg.size());
            Mat lap = currentImg - up;
            lapPyr.push_back(lap);
            currentImg = down;
        }
        currentImg.copyTo(smallestLevel);
    }
 
    Mat_<Vec3f> reconstructImgFromLapPyramid() {
        Mat currentImg = resultSmallestLevel;
        for (int l=levels-1; l>=0; l--) {
            Mat up;

            pyrUp(currentImg, up, resultLapPyr[l].size());
            currentImg = up + resultLapPyr[l];
        }
        return currentImg;
    }
 
    void blendLapPyrs() {
        resultSmallestLevel = leftSmallestLevel.mul(maskGaussianPyramid.back()) +
                                    rightSmallestLevel.mul(Scalar(1.0,1.0,1.0) - maskGaussianPyramid.back());
        for (int l=0; l<levels; l++) {
            Mat A = leftLapPyr[l].mul(maskGaussianPyramid[l]);
            Mat antiMask = Scalar(1.0,1.0,1.0) - maskGaussianPyramid[l];
            Mat B = rightLapPyr[l].mul(antiMask);
            Mat_<Vec3f> blendedLevel = A + B;
 
            resultLapPyr.push_back(blendedLevel);
        }
	}
 
public:
    LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, const Mat_<float>& _blendMask, int _levels):
    left(_left),right(_right),blendMask(_blendMask),levels(_levels){
        assert(_left.size() == _right.size());
        assert(_left.size() == _blendMask.size());
        buildPyramids();
        blendLapPyrs();
    };
 
    Mat_<Vec3f> blend() {
        return reconstructImgFromLapPyramid(); 
    }
};