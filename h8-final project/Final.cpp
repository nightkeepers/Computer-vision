#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <queue>
#include <sstream>

#include "Final.h"



using namespace std;
using namespace cimg_library;

Final::Final(string filename, int pic){
	int i;
	ratio = 3;
	pics = pic;
	CImg<unsigned char> image2;
	image.load_bmp(filename.c_str());
	int w = image.width();
	int h = image.height();
	image.resize(w * 0.5, h * 0.5);
	//image.rotate(90);
	//image.display();
	Hough houghi;
	CImg<unsigned char> Iedge = houghi.edgeDetect(image);
	houghi.generateHough();
	houghi.findTopK();
	houghi.drawLine();
	houghi.drawIntersections();
	points3 = houghi.points3;
	Homography htmp = getHomography();
	CImg<unsigned char> result = getImage(htmp, image);
	/*CImg<unsigned char> result2 = houghi.toGrayScale(result);
	CImg<unsigned char> result3 = result.blur(1);
	CImg<unsigned char> result4 = houghi.sobel(result3);
	result4.display();*/
	CImg<unsigned char> bin = binaryzation(result);
	CImg<unsigned char> extendImg = extend(bin);
	vector<vector<int>> rects = cutNumber(extendImg);
	sortNumber(rects, extendImg);




	

	/*image2 = KMeans(image);
	CImg<unsigned char> iEdge = detectEdge(image2);
	CImg<unsigned char> houghS = generateHough(iEdge);
	findTopK(houghS);
	getPoint(iEdge);
	Homography htmp = getHomography();
	cout << htmp.c1 << " " << htmp.c2 << " " << htmp.c3 << " " << htmp.c4 << " " << htmp.c5 << " " << htmp.c6 << " "<< htmp.c7 << " " << htmp.c8 << endl;
	CImg<unsigned char> result = getImage(htmp, image);*/
}

Final::Homography Final::getHomography() {
	sort(points3.begin(), points3.end());
	/*float u0 = points3[0].x, v0 = points3[0].y;
	float u1 = points3[1].x, v1 = points3[1].y;
	float u2 = points3[2].x, v2 = points3[2].y;
	float u3 = points3[3].x, v3 = points3[3].y;

	int ratio = 2;
	float x0 = 0, y0 = 0;
	float x1 = 210 * ratio, y1 = 0;
	float x2 =0 , y2 = 299 * ratio;
	float x3 = 210 * ratio, y3 = 299 * ratio;*/


	float u0 = 0, v0 = 0;
	float u1 = 210 * ratio, v1 = 0;
	float u2 = 0, v2 = 299 * ratio;
	float u3 = 210 * ratio, v3 = 299 * ratio;
	int ext = -2;

	int dis1 = (points3[0].x - points3[1].x) * (points3[0].x - points3[1].x) + (points3[0].y - points3[1].y) * (points3[0].y - points3[1].y);
	int dis2 = (points3[0].x - points3[2].x) * (points3[0].x - points3[2].x) + (points3[0].y - points3[2].y) * (points3[0].y - points3[2].y);
	if (dis1 > dis2)
	{
		node tmp = points3[1];
		points3[1] = points3[2];
		points3[2] = tmp;
	}
	float x0 = max(points3[0].x + ext, 0), y0 = max(points3[0].y + ext, 0);
	float x1 = min(points3[1].x - ext, image.width()), y1 = max(points3[1].y + ext, 0);
	float x2 = max(points3[2].x + ext, 0), y2 = min(points3[2].y - ext, image.height());
	float x3 = min(points3[3].x - ext, image.width()), y3 = min(points3[3].y - ext, image.height());
	for (int i = 0; i < 4; i++)
	{
		cout << points3[i].x << " " << points3[i].y << endl;
	}


	float c1, c2, c3, c4, c5, c6, c7, c8;

	c1 = -(u0*v0*v1*x2 - u0 * v0*v2*x1 - u0 * v0*v1*x3 + u0 * v0*v3*x1 - u1 * v0*v1*x2 + u1 * v1*v2*x0 + u0 * v0*v2*x3 - u0 * v0*v3*x2 + u1 * v0*v1*x3 - u1 * v1*v3*x0 + u2 * v0*v2*x1 - u2 * v1*v2*x0
		- u1 * v1*v2*x3 + u1 * v1*v3*x2 - u2 * v0*v2*x3 + u2 * v2*v3*x0 - u3 * v0*v3*x1 + u3 * v1*v3*x0 + u2 * v1*v2*x3 - u2 * v2*v3*x1 + u3 * v0*v3*x2 - u3 * v2*v3*x0 - u3 * v1*v3*x2 + u3 * v2*v3*x1)
		/ (u0*u1*v0*v2 - u0 * u2*v0*v1 - u0 * u1*v0*v3 - u0 * u1*v1*v2 + u0 * u3*v0*v1 + u1 * u2*v0*v1 + u0 * u1*v1*v3 + u0 * u2*v0*v3 + u0 * u2*v1*v2 - u0 * u3*v0*v2 - u1 * u2*v0*v2 - u1 * u3*v0*v1
			- u0 * u2*v2*v3 - u0 * u3*v1*v3 - u1 * u2*v1*v3 + u1 * u3*v0*v3 + u1 * u3*v1*v2 + u2 * u3*v0*v2 + u0 * u3*v2*v3 + u1 * u2*v2*v3 - u2 * u3*v0*v3 - u2 * u3*v1*v2 - u1 * u3*v2*v3 + u2 * u3*v1*v3);

	c2 = (u0*u1*v0*x2 - u0 * u2*v0*x1 - u0 * u1*v0*x3 - u0 * u1*v1*x2 + u0 * u3*v0*x1 + u1 * u2*v1*x0 + u0 * u1*v1*x3 + u0 * u2*v0*x3 + u0 * u2*v2*x1 - u0 * u3*v0*x2 - u1 * u2*v2*x0 - u1 * u3*v1*x0
		- u0 * u2*v2*x3 - u0 * u3*v3*x1 - u1 * u2*v1*x3 + u1 * u3*v1*x2 + u1 * u3*v3*x0 + u2 * u3*v2*x0 + u0 * u3*v3*x2 + u1 * u2*v2*x3 - u2 * u3*v2*x1 - u2 * u3*v3*x0 - u1 * u3*v3*x2 + u2 * u3*v3*x1)
		/ (u0*u1*v0*v2 - u0 * u2*v0*v1 - u0 * u1*v0*v3 - u0 * u1*v1*v2 + u0 * u3*v0*v1 + u1 * u2*v0*v1 + u0 * u1*v1*v3 + u0 * u2*v0*v3 + u0 * u2*v1*v2 - u0 * u3*v0*v2 - u1 * u2*v0*v2 - u1 * u3*v0*v1
			- u0 * u2*v2*v3 - u0 * u3*v1*v3 - u1 * u2*v1*v3 + u1 * u3*v0*v3 + u1 * u3*v1*v2 + u2 * u3*v0*v2 + u0 * u3*v2*v3 + u1 * u2*v2*v3 - u2 * u3*v0*v3 - u2 * u3*v1*v2 - u1 * u3*v2*v3 + u2 * u3*v1*v3);

	c3 = (u0*v1*x2 - u0 * v2*x1 - u1 * v0*x2 + u1 * v2*x0 + u2 * v0*x1 - u2 * v1*x0 - u0 * v1*x3 + u0 * v3*x1 + u1 * v0*x3 - u1 * v3*x0 - u3 * v0*x1 + u3 * v1*x0
		+ u0 * v2*x3 - u0 * v3*x2 - u2 * v0*x3 + u2 * v3*x0 + u3 * v0*x2 - u3 * v2*x0 - u1 * v2*x3 + u1 * v3*x2 + u2 * v1*x3 - u2 * v3*x1 - u3 * v1*x2 + u3 * v2*x1)
		/ (u0*u1*v0*v2 - u0 * u2*v0*v1 - u0 * u1*v0*v3 - u0 * u1*v1*v2 + u0 * u3*v0*v1 + u1 * u2*v0*v1 + u0 * u1*v1*v3 + u0 * u2*v0*v3 + u0 * u2*v1*v2 - u0 * u3*v0*v2 - u1 * u2*v0*v2 - u1 * u3*v0*v1
			- u0 * u2*v2*v3 - u0 * u3*v1*v3 - u1 * u2*v1*v3 + u1 * u3*v0*v3 + u1 * u3*v1*v2 + u2 * u3*v0*v2 + u0 * u3*v2*v3 + u1 * u2*v2*v3 - u2 * u3*v0*v3 - u2 * u3*v1*v2 - u1 * u3*v2*v3 + u2 * u3*v1*v3);

	c4 = (u0*u1*v0*v2*x3 - u0 * u1*v0*v3*x2 - u0 * u2*v0*v1*x3 + u0 * u2*v0*v3*x1 + u0 * u3*v0*v1*x2 - u0 * u3*v0*v2*x1 - u0 * u1*v1*v2*x3 + u0 * u1*v1*v3*x2 + u1 * u2*v0*v1*x3 - u1 * u2*v1*v3*x0 - u1 * u3*v0*v1*x2 + u1 * u3*v1*v2*x0
		+ u0 * u2*v1*v2*x3 - u0 * u2*v2*v3*x1 - u1 * u2*v0*v2*x3 + u1 * u2*v2*v3*x0 + u2 * u3*v0*v2*x1 - u2 * u3*v1*v2*x0 - u0 * u3*v1*v3*x2 + u0 * u3*v2*v3*x1 + u1 * u3*v0*v3*x2 - u1 * u3*v2*v3*x0 - u2 * u3*v0*v3*x1 + u2 * u3*v1*v3*x0)
		/ (u0*u1*v0*v2 - u0 * u2*v0*v1 - u0 * u1*v0*v3 - u0 * u1*v1*v2 + u0 * u3*v0*v1 + u1 * u2*v0*v1 + u0 * u1*v1*v3 + u0 * u2*v0*v3 + u0 * u2*v1*v2 - u0 * u3*v0*v2 - u1 * u2*v0*v2 - u1 * u3*v0*v1
			- u0 * u2*v2*v3 - u0 * u3*v1*v3 - u1 * u2*v1*v3 + u1 * u3*v0*v3 + u1 * u3*v1*v2 + u2 * u3*v0*v2 + u0 * u3*v2*v3 + u1 * u2*v2*v3 - u2 * u3*v0*v3 - u2 * u3*v1*v2 - u1 * u3*v2*v3 + u2 * u3*v1*v3);

	c5 = -(u0*v0*v1*y2 - u0 * v0*v2*y1 - u0 * v0*v1*y3 + u0 * v0*v3*y1 - u1 * v0*v1*y2 + u1 * v1*v2*y0 + u0 * v0*v2*y3 - u0 * v0*v3*y2 + u1 * v0*v1*y3 - u1 * v1*v3*y0 + u2 * v0*v2*y1 - u2 * v1*v2*y0
		- u1 * v1*v2*y3 + u1 * v1*v3*y2 - u2 * v0*v2*y3 + u2 * v2*v3*y0 - u3 * v0*v3*y1 + u3 * v1*v3*y0 + u2 * v1*v2*y3 - u2 * v2*v3*y1 + u3 * v0*v3*y2 - u3 * v2*v3*y0 - u3 * v1*v3*y2 + u3 * v2*v3*y1)
		/ (u0*u1*v0*v2 - u0 * u2*v0*v1 - u0 * u1*v0*v3 - u0 * u1*v1*v2 + u0 * u3*v0*v1 + u1 * u2*v0*v1 + u0 * u1*v1*v3 + u0 * u2*v0*v3 + u0 * u2*v1*v2 - u0 * u3*v0*v2 - u1 * u2*v0*v2 - u1 * u3*v0*v1
			- u0 * u2*v2*v3 - u0 * u3*v1*v3 - u1 * u2*v1*v3 + u1 * u3*v0*v3 + u1 * u3*v1*v2 + u2 * u3*v0*v2 + u0 * u3*v2*v3 + u1 * u2*v2*v3 - u2 * u3*v0*v3 - u2 * u3*v1*v2 - u1 * u3*v2*v3 + u2 * u3*v1*v3);

	c6 = (u0*u1*v0*y2 - u0 * u2*v0*y1 - u0 * u1*v0*y3 - u0 * u1*v1*y2 + u0 * u3*v0*y1 + u1 * u2*v1*y0 + u0 * u1*v1*y3 + u0 * u2*v0*y3 + u0 * u2*v2*y1 - u0 * u3*v0*y2 - u1 * u2*v2*y0 - u1 * u3*v1*y0
		- u0 * u2*v2*y3 - u0 * u3*v3*y1 - u1 * u2*v1*y3 + u1 * u3*v1*y2 + u1 * u3*v3*y0 + u2 * u3*v2*y0 + u0 * u3*v3*y2 + u1 * u2*v2*y3 - u2 * u3*v2*y1 - u2 * u3*v3*y0 - u1 * u3*v3*y2 + u2 * u3*v3*y1)
		/ (u0*u1*v0*v2 - u0 * u2*v0*v1 - u0 * u1*v0*v3 - u0 * u1*v1*v2 + u0 * u3*v0*v1 + u1 * u2*v0*v1 + u0 * u1*v1*v3 + u0 * u2*v0*v3 + u0 * u2*v1*v2 - u0 * u3*v0*v2 - u1 * u2*v0*v2 - u1 * u3*v0*v1
			- u0 * u2*v2*v3 - u0 * u3*v1*v3 - u1 * u2*v1*v3 + u1 * u3*v0*v3 + u1 * u3*v1*v2 + u2 * u3*v0*v2 + u0 * u3*v2*v3 + u1 * u2*v2*v3 - u2 * u3*v0*v3 - u2 * u3*v1*v2 - u1 * u3*v2*v3 + u2 * u3*v1*v3);

	c7 = (u0*v1*y2 - u0 * v2*y1 - u1 * v0*y2 + u1 * v2*y0 + u2 * v0*y1 - u2 * v1*y0 - u0 * v1*y3 + u0 * v3*y1 + u1 * v0*y3 - u1 * v3*y0 - u3 * v0*y1 + u3 * v1*y0
		+ u0 * v2*y3 - u0 * v3*y2 - u2 * v0*y3 + u2 * v3*y0 + u3 * v0*y2 - u3 * v2*y0 - u1 * v2*y3 + u1 * v3*y2 + u2 * v1*y3 - u2 * v3*y1 - u3 * v1*y2 + u3 * v2*y1)
		/ (u0*u1*v0*v2 - u0 * u2*v0*v1 - u0 * u1*v0*v3 - u0 * u1*v1*v2 + u0 * u3*v0*v1 + u1 * u2*v0*v1 + u0 * u1*v1*v3 + u0 * u2*v0*v3 + u0 * u2*v1*v2 - u0 * u3*v0*v2 - u1 * u2*v0*v2 - u1 * u3*v0*v1
			- u0 * u2*v2*v3 - u0 * u3*v1*v3 - u1 * u2*v1*v3 + u1 * u3*v0*v3 + u1 * u3*v1*v2 + u2 * u3*v0*v2 + u0 * u3*v2*v3 + u1 * u2*v2*v3 - u2 * u3*v0*v3 - u2 * u3*v1*v2 - u1 * u3*v2*v3 + u2 * u3*v1*v3);

	c8 = (u0*u1*v0*v2*y3 - u0 * u1*v0*v3*y2 - u0 * u2*v0*v1*y3 + u0 * u2*v0*v3*y1 + u0 * u3*v0*v1*y2 - u0 * u3*v0*v2*y1 - u0 * u1*v1*v2*y3 + u0 * u1*v1*v3*y2 + u1 * u2*v0*v1*y3 - u1 * u2*v1*v3*y0 - u1 * u3*v0*v1*y2 + u1 * u3*v1*v2*y0
		+ u0 * u2*v1*v2*y3 - u0 * u2*v2*v3*y1 - u1 * u2*v0*v2*y3 + u1 * u2*v2*v3*y0 + u2 * u3*v0*v2*y1 - u2 * u3*v1*v2*y0 - u0 * u3*v1*v3*y2 + u0 * u3*v2*v3*y1 + u1 * u3*v0*v3*y2 - u1 * u3*v2*v3*y0 - u2 * u3*v0*v3*y1 + u2 * u3*v1*v3*y0)
		/ (u0*u1*v0*v2 - u0 * u2*v0*v1 - u0 * u1*v0*v3 - u0 * u1*v1*v2 + u0 * u3*v0*v1 + u1 * u2*v0*v1 + u0 * u1*v1*v3 + u0 * u2*v0*v3 + u0 * u2*v1*v2 - u0 * u3*v0*v2 - u1 * u2*v0*v2 - u1 * u3*v0*v1
			- u0 * u2*v2*v3 - u0 * u3*v1*v3 - u1 * u2*v1*v3 + u1 * u3*v0*v3 + u1 * u3*v1*v2 + u2 * u3*v0*v2 + u0 * u3*v2*v3 + u1 * u2*v2*v3 - u2 * u3*v0*v3 - u2 * u3*v1*v2 - u1 * u3*v2*v3 + u2 * u3*v1*v3);

	return Homography(c1, c2, c3, c4, c5, c6, c7, c8);
}


CImg<unsigned char> Final::getImage(Homography h, CImg<unsigned char> img)
{
	CImg<unsigned char> I2 = CImg<unsigned char>(210 * ratio, 299 * ratio, 1, 3, 0);
	cimg_forXY(I2, x, y) {
		int x1 = (h.c1* x + h.c2 * y + h.c3 * x * y + h.c4);
		int y1 = (h.c5* x + h.c6 * y + h.c7 * x * y + h.c8);
		if (x1 >= 0 && x1 < img.width() && y1>0 && y1 < img.height())
		{
			I2(x, y, 0) = img(x1, y1, 0);
			I2(x, y, 1) = img(x1, y1, 1);
			I2(x, y, 2) = img(x1, y1, 2);
		}
	}
	I2.display();
	return I2;
}

CImg<unsigned char> Final::binaryzation(CImg<unsigned char> img)
{
	int i, j;
	CImg<unsigned char> newIm = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
	CImg<unsigned char> isboundary = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
	cimg_forXY(newIm, x, y)
	{
		if (img(x, y) < 140)
		{
			if (x == 0 || x == img.width())
				isboundary(x, y) = 1;
			else if(y == 0 || y == img.height())
				isboundary(x, y) = 1;
			else {
				for (i = -1; i < 2; i++)
				{
					for (j = -1; j < 2; j++)
					{
						if (x + i < 0 || y + j < 0 || x + i >= img.width() || y + j >= img.height())
							continue;
						if (isboundary(x+i,y+j)==1)
							isboundary(x, y) = 1;
					}
				}
			}
		}
		if (img(x, y) < 200)
		{
			if (isboundary(x, y) == 0)
			{
				int sum = 0, cnt = 0;
				float mean, vara = 0, std;
				for (i = -10; i < 11; i++)
				{
					for (j = -10; j < 11; j++)
					{
						if (x + i < 0 || y + j < 0 || x + i >= img.width() || y + j >= img.height())
							continue;
						sum += img(x + i, y + j);
						cnt++;
					}
				}
				mean = (float)sum / cnt;
				for (i = -10; i < 11; i++)
				{
					for (j = -10; j < 11; j++)
					{
						if (x + i < 0 || y + j < 0 || x + i >= img.width() || y + j >= img.height())
							continue;
						vara += (img(x + i, y + j) - mean)*(img(x + i, y + j) - mean);
					}
				}
				std = sqrt(vara) / cnt;
				if (img(x, y) < mean*(1 + 0.1*(std / 128 - 1)))
					newIm(x, y) = 255;
			}
		}
	}
	//newIm.display();
	return newIm;
}


CImg<unsigned char> Final::extend(CImg<unsigned char> img)
{
	int i, j;
	CImg<unsigned char> newIm = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
	CImg<unsigned char> newIm2 = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
	CImg<unsigned char> newIm3 = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
	cimg_forXY(newIm, x, y)
	{
		int whiteAccu = 0;
		if (img(x, y) == 0)
		{
			for (i = -1; i < 2; i++)
			{
				if (y + i >= 0 && y + i < img.height()) {
					if (img(x, y + i) == 255)
						whiteAccu++;
				}
			}
			for (i = -2; i <= 2; i++) {
				if (0 <= x + i && x + i < img.width()) {  
					if (img(x + i, y) == 255)
						whiteAccu--;
				}
			}
			if (whiteAccu > 0)
				newIm(x, y) = 255;
		}
		else
			newIm(x, y) = 255;

	}
	newIm2 = newIm;
	/*cimg_forXY(newIm2, x, y)
	{
		int whiteAccu = 0;
		if (newIm(x, y) == 0)
		{
			for (i = -1; i < 2; i++)
			{
				if (y + i >= 0 && y + i < img.height()) {
					if (newIm(x, y + i) == 255)
						whiteAccu++;
				}
			}
			for (i = -2; i <= 2; i++) {
				if (0 <= x + i && x + i < img.width()) {   
					if (newIm(x + i, y) == 255)
						whiteAccu--;
				}
			}
			if (whiteAccu > 0)
				newIm2(x, y) = 255;
		}
		else
			newIm2(x, y) = 255;

	}*/
	cimg_forXY(newIm3, x, y)
	{
		if (newIm2(x, y) == 0)
		{
			for (i = -1; i < 2; i++)
			{
				for (j = -1; j < 2; j++)
				{
					if (x + i < 0 || y + j < 0 || x + i >= img.width() || y + j >= img.height())
						continue;
					//if ((i != -1 || j != -1) &&( i != 1 || j != 1) && (i != 1 || j != -1 )&&( i != -1 || j != 1))
					if (i != -1 && j != -1 || i != 1 && j != 1 || i != 1 && j != -1 || i != -1 && j != 1)
					{
						if (newIm2(x + i, y + j) ==255) {
							newIm3(x,y) = 255;
							break;
						}
					}
				}
				if (newIm3(x, y) != 0)
					break;
			}
		}
		else
			newIm3(x, y) = 255;
	}
	newIm3.display();
	return newIm3;
}

vector<vector<int>> Final::cutNumber(CImg<unsigned char> img)
{
	int i, j, sumh = 0, cnt = 0, sumw = 0;
	CImg<unsigned char> vis = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
	vector<vector<int>> result;
	cimg_forXY(img, x, y)
	{
		if (vis(x,y) == 0 && img(x, y) == 255)
		{
			int x1 = x, y1 = y, x2 = x, y2 = y;
			queue<node> bfs;
			node temp;
			temp.x = x;
			temp.y = y;
			temp.value = 0;
			bfs.push(temp);
			vis(x, y) = 1;
			while (!bfs.empty())
			{
				node temp2 = bfs.front();
				bfs.pop();
				x1 = min(x1, temp2.x);
				y1 = min(y1, temp2.y);
				x2 = max(x2, temp2.x);
				y2 = max(y2, temp2.y);
				if (temp2.value > 0)
				{
					if (cnt > 5)
					{
						if (y2 - y1 > sumh / cnt + 5)
						{
							if (temp2.value >= 3)
								continue;
						}
						if (y2 - y1 > sumh / cnt + 10)
						{
							if (temp2.value >= 2)
								continue;
						}
						if (x2 - x1 > sumw / cnt + 5)
						{
							if (temp2.value >= 3)
								continue;
						}
						if (x2 - x1 > sumw / cnt + 10)
						{
							if (temp2.value >= 2)
								continue;
						}
					}
					if (temp2.value >= 4)
						continue;
				}
				for (i = -1; i < 2; i++)
				{
					for (j = -1; j < 2; j++)
					{
						if (temp2.x + i < 0 || temp2.y + j < 0 || temp2.x + i >= img.width() || temp2.y + j >= img.height())
							continue;
						if (vis(temp2.x + i, temp2.y + j)==0 && img(temp2.x + i, temp2.y + j) == 255)
						{
							temp.x = temp2.x + i;
							temp.y = temp2.y + j;
							temp.value = 0;
							bfs.push(temp);
							vis(temp2.x + i, temp2.y + j) = 1;
						}
						else if(vis(temp2.x + i, temp2.y + j) == 0)
						{
							temp.x = temp2.x + i;
							temp.y = temp2.y + j;
							temp.value = temp2.value + 1;
							bfs.push(temp);
							vis(temp2.x + i, temp2.y + j) = 1;
						}
					}
				}
			}
			if (x2 - x1 >= 10 && y2 - y1 >= 10&&(x2 - x1 >= 18 || y2 - y1 >= 18)&&x2-x1<100 && y2 - y1<150)
			{
				sumh += y2 - y1;
				sumw += x2 - x1;
				cnt++;
				vector<int> rect;
				rect.push_back(x1);
				rect.push_back(y1);
				rect.push_back(x2);
				rect.push_back(y2);
				result.push_back(rect);
			}
			for (i = x1; i <= x2; i++)
			{
				for (j = y1; j <= y2; j++)
				{
					vis(i, j) = 1;
				}
			}
		}
	}
	/*for (i = 0; i < result.size(); i++)
	{
		CImg<unsigned char> now = CImg<unsigned char>(result[i][2]-result[i][0], result[i][3] - result[i][1], 1, 1, 0);
		cimg_forXY(now, x, y)
		{
			now(x, y) = img(x + result[i][0], y + result[i][1]);
		}
		now.display();
	}*/
	return result;
}

void Final::sortNumber(vector<vector<int>> rects, CImg<unsigned char> img)
{
	int i, j, k, flag = 0;
	vector<vector<int>> result;
	vector<int> highs;
	vector<int> lows;
	for (i = 0; i < rects.size(); i++)
	{
		flag = 0;
		for (j = 0; j < result.size(); j++)
		{
			int left = -1, right = -1;
			for (k = 0; k < result[j].size(); k++)
			{
				int temp = result[j][k];
				if (rects[i][0] < rects[temp][2] - 4 && rects[i][0] > rects[temp][0])
					break;
				if (rects[i][2] - 4 > rects[temp][0] && rects[i][0] < rects[temp][0])
					break;
				if (rects[i][0] > rects[temp][0])
				{
					if (left == -1 || rects[left][0] < rects[temp][0])
						left = temp;
				}
				else
				{
					if (right == -1 || rects[right][0] > rects[temp][0])
						right = temp;
				}
			}
			if (left != -1)
			{
				if (rects[left][3] < rects[i][1] + 5)
					continue;
				if (rects[left][1] > rects[i][3])
				{
					flag = 1;
					continue;
				}
			}
			if (right != -1)
			{
				if (rects[right][3] < rects[i][1] + 5)
					continue;
				if (rects[right][1] > rects[i][3])
				{
					flag = 1;
					continue;
				}
			}
			if (k == result[j].size())
			{
				result[j].push_back(i);
				break;
			}
		}
		if (!flag&&j == result.size())
		{
			vector<int> newRow;
			newRow.push_back(i);
			result.push_back(newRow);
		}
	}
	for (i = 0; i < result.size(); i++)
	{
		for (j = 0; j < result[i].size(); j++)
		{
			int temp = result[i][j];
			CImg<unsigned char> now = CImg<unsigned char>(rects[temp][2] - rects[temp][0], rects[temp][3] - rects[temp][1], 1, 1, 0);
			cimg_forXY(now, x, y)
			{
				now(x, y) = img(x + rects[temp][0], y + rects[temp][1]);
			}
			stringstream ss;
			ss << i;
			string s = "Data2/";
			stringstream ss3;
			ss3 << pics;
			s += ss3.str();
			s += "_";
			s += ss.str();
			stringstream ss2;
			ss2 << rects[temp][0];
			s += "_";
			string s2 = ss2.str();
			while (s2.size() < 3)
			{
				s2 = '0' + s2;
			}
			//cout << s2.size() << " "<< ss2.str() << endl;
			s += s2;
			s += ".bmp";
			now.save(s.c_str());
		}

	}
	//cin >> i;
}








