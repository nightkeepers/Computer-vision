#ifndef _hough_
#define _hough_
#include "CImg.h"
#include "Sift.h"
#include <vector>

using namespace std;
using namespace cimg_library;

class ImageStitching {
private:
	vector<CImg<unsigned char>> images; 
	float thresold;
	float thresold2;
	vector<CImg<unsigned char>> stitched;
public:
	ImageStitching(vector<string> filenames);
	CImg<unsigned char> toGrayScale(CImg<unsigned char> img);
	CImg<unsigned char> siftImage(CImg<unsigned char> img, CImg<unsigned char> img2);
	CImg<unsigned char> BlendImage(CImg<unsigned char> img, CImg<unsigned char> img2, float shiftX, float shiftY);
	float computeDistance(SiftDescriptor SD1, SiftDescriptor SD2);
	float getShift(vector<float> shiftV);
};



#endif