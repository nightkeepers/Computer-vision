#ifndef _Segment_
#define _Segment_
#include "CImg.h"
#include "hough.h"
#include <vector>


using namespace std;
using namespace cimg_library;

class Final{
private:
	vector<CImg<unsigned char>> images; 
	CImg<unsigned char> image;

	int ratio;
	vector<node> points3;
	int pics;
	struct Homography {
		float c1;
		float c2;
		float c3;
		float c4;
		float c5;
		float c6;
		float c7;
		float c8;
		Homography(float _c1, float _c2, float _c3, float _c4, float _c5, float _c6, float _c7, float _c8) {
			c1 = _c1;
			c2 = _c2;
			c3 = _c3;
			c4 = _c4;
			c5 = _c5;
			c6 = _c6;
			c7 = _c7;
			c8 = _c8;
		}
	};
	/*float thresold;
	float thresold2;
	vector<CImg<unsigned char>> stitched;
	int maxn;
	

	vector<double> ks;
	vector<double> bs;
	vector<double> thetas;
*/
public:
	Final(string filename, int pic);
	Homography getHomography();
	CImg<unsigned char> getImage(Homography h, CImg<unsigned char> img);
	CImg<unsigned char> binaryzation(CImg<unsigned char> img);
	CImg<unsigned char> extend(CImg<unsigned char> img);
	vector<vector<int>> cutNumber(CImg<unsigned char> img);
	void sortNumber(vector<vector<int>> rects, CImg<unsigned char> img);
	/*CImg<unsigned char> KMeans(CImg<unsigned char> img);
	float SD(int a[]);
	float computeDistance(int a[], int b[], int mode);
	CImg<unsigned char> detectEdge(CImg<unsigned char> img);
	CImg<unsigned char> generateHough(CImg<unsigned char> iEdge);
	void findTopK(CImg<unsigned char> houghS);
	void getPoint(CImg<unsigned char> iEdge);
*/
};



#endif