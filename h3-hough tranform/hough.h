#ifndef _hough_
#define _hough_
#include "CImg.h"
#include <vector>

using namespace std;
using namespace cimg_library;

class Hough {
private:
	CImg<unsigned char> img; //Original Image
	CImg<unsigned char> iEdge; //Edge Image
	CImg<unsigned char> angles; //Angle Map
	CImg<unsigned char> houghS; //hough space
	struct node {
		int x, y, value;
		bool operator < (const node &a)const
		{
			return value < a.value;
		}
	};
	vector<node> points;
	int maxn;
	CImg<unsigned char> I2; //line image
	vector<double> ks;
	vector<double> bs;
	vector<double> thetas;
	CImg<unsigned char> I3; 
	vector<double> xs;
	vector<double> ys;
	CImg<unsigned char> I4; 

	//used for circle transform
	CImg<unsigned char> houghS2; //hough space
	vector<node> nodes;
	vector<node> circles;
	vector<node> center;
public :
	Hough(char filename[], int mode);
	CImg<unsigned char> toGrayScale(CImg<unsigned char> src);
	CImg<unsigned char> edgeDetect(CImg<unsigned char>);
	CImg<unsigned char> sobel(CImg<unsigned char>);
	CImg<unsigned char> nonMaxSupp(CImg<unsigned char>); //Non-maxima supp.
	CImg<unsigned char> threshold(CImg<unsigned char> imgin, int low, int high);
	CImg<unsigned char> generateHough();
	void findTopK();
	CImg<unsigned char> drawLine();
	void drawIntersections();
	void drawEdge();
	void houghCircles(CImg<unsigned char>& img, int minR, int maxR);
	void drawCircle(int r);
};



#endif