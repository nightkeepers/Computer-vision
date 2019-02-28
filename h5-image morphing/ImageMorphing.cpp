#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include "CImg.h"
#include "ImageMorphing.h"


using namespace std;
using namespace cimg_library;

ImageMorphing::ImageMorphing(char filename[], char filename2[]) {
	img.load_bmp(filename);
	target.load_bmp(filename2);
	frame = 11;
	getTriangles();
	morphing();

}

void ImageMorphing::getTriangles()
{
	fstream infile("data/2.txt"),infile2("data/1.txt"),infile3("data/3.txt");
	string temp;
	point p;
	int x, y, z, i, j;
	while (getline(infile,temp))
	{
		stringstream ss(temp);
		ss >> x >> y;
		p.x = x;
		p.y = y;
		points1.push_back(p);
	}
	cout << points1.size()<<endl;
	infile.close();
	while (getline(infile2, temp))
	{
		stringstream ss(temp);
		ss >> x >> y;
		point p;
		p.x = x;
		p.y = y;
		points2.push_back(p);
	}
	infile2.close();
	while (getline(infile3, temp))
	{
		stringstream ss(temp);
		ss >> x >> y >> z;
		vector<int> t;
		t.push_back(x);
		t.push_back(y);
		t.push_back(z);
		tri.push_back(t);
	}
	infile3.close();
	for (i = 0; i < tri.size(); i++)
	{
		triangle tmp;
		tmp.p1 = points1[tri[i][0]-1];
		tmp.p2 = points1[tri[i][1]-1];
		tmp.p3 = points1[tri[i][2]-1];
		srcT.push_back(tmp);
	}
	for (i = 0; i < tri.size(); i++)
	{
		triangle tmp;
		tmp.p1 = points2[tri[i][0]-1];
		tmp.p2 = points2[tri[i][1]-1];
		tmp.p3 = points2[tri[i][2]-1];
		tarT.push_back(tmp);
	}
	for (i = 0; i < frame; i++)
	{
		vector<point> ps;
		vector<triangle> ts;
		for (j = 0; j < points1.size(); j++)
		{
			float mx = (float)(points1[j].x) + (float)(i + 1) * (points2[j].x - points1[j].x) / (frame + 1);
			float my = (float)(points1[j].y) + (float)(i + 1) * (points2[j].y - points1[j].y) / (frame + 1);
			p.x = mx;
			p.y = my;
			ps.push_back(p);
		}
		points3.push_back(ps);
		for (j = 0; j < tri.size(); j++)
		{
			triangle tmp;
			tmp.p1 = ps[tri[j][0]-1];
			tmp.p2 = ps[tri[j][1]-1];
			tmp.p3 = ps[tri[j][2]-1];
			ts.push_back(tmp);
		}
		midT.push_back(ts);
	}
}


bool ImageMorphing::isInTriangle(triangle t, point p) {
	float x31 = t.p3.x - t.p1.x, y31 = t.p3.y - t.p1.y;
	float x21 = t.p2.x - t.p1.x, y21 = t.p2.y - t.p1.y;
	float xp1 = p.x - t.p1.x, yp1 = p.y - t.p1.y;

	float temp1 = x31 * x31 + y31 * y31;
	float temp2 = x31 * x21 + y31 * y21;
	float temp3 = x31 * xp1 + y31 * yp1;
	float temp4 = x21 * x21 + y21 * y21;
	float temp5 = x21 * xp1 + y21 * yp1;

	float u = float(temp4 * temp3 - temp2 * temp5) / (float)(temp1 * temp4 - temp2 * temp2);
	float v = float(temp1 * temp5 - temp2 * temp3) / (float)(temp1 * temp4 - temp2 * temp2);
	if (u + v <= 1 && u >= 0 && v >= 0) return true;
	return false;

}

/*bool ImageMorphing::isInTriangle(triangle tri,point P) {
	float x0 = tri.p3.x - tri.p1.x, y0 = tri.p3.y - tri.p1.y;
	float x1 = tri.p2.x - tri.p1.x, y1 = tri.p2.y - tri.p1.y;
	float x2 = P.x - tri.p1.x, y2 = P.y - tri.p1.y;

	float temp_00 = x0 * x0 + y0 * y0;
	float temp_01 = x0 * x1 + y0 * y1;
	float temp_02 = x0 * x2 + y0 * y2;
	float temp_11 = x1 * x1 + y1 * y1;
	float temp_12 = x1 * x2 + y1 * y2;

	float u = float(temp_11 * temp_02 - temp_01 * temp_12) / (float)(temp_00 * temp_11 - temp_01 * temp_01);
	float v = float(temp_00 * temp_12 - temp_01 * temp_02) / (float)(temp_00 * temp_11 - temp_01 * temp_01);
	if (u + v <= 1 && u >= 0 && v >= 0) return true;
	return false;
}*/


CImg<float> ImageMorphing::computeMatrix(triangle t1, triangle t2){
	int i;
	CImg<float> A(3, 3, 1, 1, 1);
	CImg<float> y1(1, 3, 1, 1, 0), y2(1, 3, 1, 1, 0);
	CImg<float> c1(1, 3, 1, 1, 0), c2(1, 3, 1, 1, 0);
	CImg<float> t(3, 3, 1, 1, 0);
	A(0, 0) = t1.p1.x; A(1, 0) = t1.p1.y;
	A(0, 1) = t1.p2.x; A(1, 1) = t1.p2.y;
	A(0, 2) = t1.p3.x; A(1, 2) = t1.p3.y;
	y1(0, 0) = t2.p1.x; y2(0, 0) = t2.p1.y;
	y1(0, 1) = t2.p2.x; y2(0, 1) = t2.p2.y;
	y1(0, 2) = t2.p3.x; y2(0, 2) = t2.p3.y;
	c1 = y1.solve(A);
	c2 = y2.solve(A);
	for (i = 0; i < 3; i++) {
		t(i, 0) = c1(0, i);
		t(i, 1) = c2(0, i);
	}
	t(2, 2) = 1;
	return t;
}

void ImageMorphing::morphing()
{
	int i,j;
	int size = midT[0].size();
	ans.push_back(img);
	point p;
	CImg<unsigned char> temp(target.width(), target.height(), 1, 3, 1);
	for (i = 0; i < frame; i++)
	{
		cimg_forXY(temp, x, y) {
			CImg<float> x1(1, 3, 1, 1, 1), y1(1, 3, 1, 1, 1), y2(1, 3, 1, 1, 1);
			for (j = 0; j < size; j++)
			{
				p.x = x;
				p.y = y;
				if (isInTriangle(midT[i][j],p))
				{
					x1(0, 0) = x;
					x1(0, 1) = y;
					CImg<float> t1 = computeMatrix(midT[i][j], srcT[j]);
					CImg<float> t2 = computeMatrix(midT[i][j], tarT[j]);
					y1 = t1 * x1;
					y2 = t2 * x1;
					float r = float(i + 1) / (frame + 1);
					float temp0 = y2(0, 0);
					float temp1 = y2(0, 1);
					temp(x, y, 0) = (1 - r) * img(y1(0, 0), y1(0, 1), 0) + r * target(y2(0, 0), y2(0, 1), 0);
					temp(x, y, 1) = (1 - r) * img(y1(0, 0), y1(0, 1), 1) + r * target(y2(0, 0), y2(0, 1), 1);
					temp(x, y, 2) = (1 - r) * img(y1(0, 0), y1(0, 1), 2) + r * target(y2(0, 0), y2(0, 1), 2);
				}
			}
		}
		ans.push_back(temp);
		string dir = "image/" + to_string(i + 1) + ".bmp";
		temp.save_bmp(dir.c_str());
	}
	ans.push_back(target);
	ans.save_gif_external("image/result.gif");
}

