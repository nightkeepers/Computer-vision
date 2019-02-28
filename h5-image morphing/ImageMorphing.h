#ifndef _hough_
#define _hough_
#include "CImg.h"
#include <vector>

using namespace std;
using namespace cimg_library;

class ImageMorphing {
private:
	CImg<unsigned char> img; //Original Image
	CImg<unsigned char> target;
	struct point {
		float x, y;
	};
	struct triangle {
		point p1, p2, p3;
	};
	vector<point> points1;
	vector<point> points2;
	vector<vector<point>> points3;
	vector<vector<int>> tri;

	vector<triangle> srcT;
	vector<triangle> tarT;
	vector<vector<triangle>> midT;
	int frame;
	CImgList<unsigned char> ans;
public:
	ImageMorphing(char filename[], char filename2[]);
	void getTriangles();
	bool isInTriangle(triangle t, point p);
	CImg<float> computeMatrix(triangle t1, triangle t2);
	void morphing();
};



#endif