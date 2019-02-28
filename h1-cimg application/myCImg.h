#include "CImg.h"
#include <cmath>
#define pi 3.1415926

using namespace cimg_library; 

class myCImg {
public:
	CImg<unsigned char> image;
	myCImg();
	void display();
	void changeColor();
	void drawCircle();
	void drawCircle2();
	void drawCircleY();
	void drawCircleY2();
	void drawLine();
	void drawLine2();
	void save(int i);
};
