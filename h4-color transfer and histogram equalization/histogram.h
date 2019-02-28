#ifndef _hough_
#define _hough_
#include "CImg.h"
#include <vector>

using namespace std;
using namespace cimg_library;

class Histogram {
private:
	CImg<unsigned char> img; //Original Image
	CImg<unsigned char> src;
	CImg<unsigned char> target;
public :
	Histogram() {};
	Histogram(char filename[]);
	void setImg(char filename1[], char filename2[]);
	CImg<unsigned char> toGrayScale(CImg<unsigned char> src);
	CImg<unsigned char> equalization(CImg<unsigned char> src);
	CImg<float> transfer();
};



#endif