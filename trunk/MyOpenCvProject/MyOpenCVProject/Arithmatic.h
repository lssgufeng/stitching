#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"

/***
* class Arithmatic
* description This class is targetted for getting statistical 
  & arithmatic operated image. 
* author Krishna Paudel(krishna444@gmail.com)
* date 2012-04-27 11:50am
***/
class Arithmatic{	

private:

public:
	//Constructor 
	Arithmatic();
	//Destructor
	~Arithmatic();

	//Calculate the standard deviation 
	double CalculateSD(cv::Mat& image);
	double CalculateAverage(cv::Mat& image);
};

//Declarations
inline Arithmatic::Arithmatic(){
}
inline Arithmatic::~Arithmatic(){
}


inline double Arithmatic::CalculateAverage(cv::Mat& image){
	double average=0.00;
	int nl=image.rows;
	int nc=image.cols;

	for(int j=0;j<nl;j++){
		ushort* data=image.ptr<ushort>(j);
		for(int i=0;i<nc;i++){
			average+=(*data+i)/((double)nl*nc);
		}
		//average/=nl*nc;
	}
	return average;
}

inline double Arithmatic::CalculateSD(cv::Mat& image){
	double sd=0.00;
	double average=CalculateAverage(image);
	printf("Average=%f",average);
    int nl=image.rows;	
	int nc=image.cols;
	for(int j=0;j<nl;j++){
		ushort* data=image.ptr<ushort>(j);
		for(int i=0;i<nc;i++){
			sd+=(*(data+i)-average)*((*data+i)-average)/(nl*nc);
		}
	}
	sd=cv::sqrt(sd);

	//printf("SD=%f",sd);

	return sd;
}