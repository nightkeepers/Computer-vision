#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "ImageSegmentation.h"


using namespace std;
using namespace cimg_library;

ImageSegmentation::ImageSegmentation(string filename){
	int i;
	ratio = 2;
	
	CImg<unsigned char> image2;
	image.load_bmp(filename.c_str());
	image2 = KMeans(image);
	CImg<unsigned char> iEdge = detectEdge(image2);
	CImg<unsigned char> houghS = generateHough(iEdge);
	findTopK(houghS);
	getPoint(iEdge);
	Homography htmp = getHomography();
	cout << htmp.c1 << " " << htmp.c2 << " " << htmp.c3 << " " << htmp.c4 << " " << htmp.c5 << " " << htmp.c6 << " "<< htmp.c7 << " " << htmp.c8 << endl;
	CImg<unsigned char> result = getImage(htmp, image);
}

CImg<unsigned char> ImageSegmentation::KMeans(CImg<unsigned char> img)
{
	int i, j, p, k = 2, mincost = -1, mode = 3, curcost = 0, flag2 = 0;
	int centers[5][5], cnt[5], total[5][5], temp[5];
	CImg<unsigned char> newIm = img;
	CImg<unsigned char> bestIm = img;
	for (i = 0; i < 10; i++)
	{
		curcost = 0;
		flag2 = 0;
		for (j = 0; j < k; j++)
		{
			for (p = 0; p < mode; p++)
			{
				centers[j][p] = rand() % 256;
				if(p==4)
					centers[j][p] = rand() % 30;
				if(p==5)
					centers[j][p] = rand() % img.height();
			}
		}
		while (true)
		{
			int flag = 0;
			memset(cnt, 0, sizeof(cnt));
			memset(total, 0, sizeof(total));
			cimg_forXY(img, x, y) {
				
				int center = 0;
				float mindis = -1;
				temp[0] = img(x, y, 0);
				temp[1] = img(x, y, 1);
				temp[2] = img(x, y, 2);
				temp[3] = SD(temp);
				temp[4] = y;
				for (j = 0; j < k; j++)
				{
					float dis = computeDistance(centers[j], temp, mode);
					if (mindis == -1 || mindis > dis)
					{
						center = j;
						mindis = dis;
					}
				}
				newIm(x, y, 0) = center;
			}
			cimg_forXY(newIm, x, y){
				int center = newIm(x, y, 0);
				cnt[center]++;
				temp[0] = img(x, y, 0);
				temp[1] = img(x, y, 1);
				temp[2] = img(x, y, 2);
				total[center][0] += img(x, y, 0);
				total[center][1] += img(x, y, 1);
				total[center][2] += img(x, y, 2);
				total[center][3] += SD(temp);
				total[center][4] += y;

			}
			for (j = 0; j < k; j++)
			{
				if (cnt[j] == 0)
				{
					flag2 = 1;
					break;
				}
				for (p = 0; p < mode; p++)
				{
					int tmp = total[j][p] / cnt[j];
					if (tmp != centers[j][p])
						flag = 1;
					centers[j][p] = tmp;
					cout << centers[j][p] << " ";
				}
				cout << endl;
			}
			cout << endl;
			if (flag == 0)
				break;
		}
		if (flag2 == 1)
			continue;
		cimg_forXY(newIm, x, y) {
			/*if (x == 200 && y % 10 == 0)
				cout << int(img(x, y, 0)) << " " << int(img(x, y, 1)) << " " << int(img(x, y, 2))<<endl;*/
			int center = newIm(x, y, 0);
			temp[0] = img(x, y, 0);
			temp[1] = img(x, y, 1);
			temp[2] = img(x, y, 2);
			temp[3] = SD(temp);
			temp[4] = y;
			int cost = pow(computeDistance(centers[j], temp, mode), 2);
			curcost += cost;
		}
		if (mincost == -1 || mincost > curcost)
		{
			mincost = curcost;
			cimg_forXY(bestIm, x, y) {
				int center = newIm(x, y, 0);
				bestIm(x, y, 0) = centers[center][0];
				bestIm(x, y, 1) = centers[center][1];
				bestIm(x, y, 2) = centers[center][2];
			}
		}
	}
	bestIm.display();
	return bestIm;
}

float ImageSegmentation::computeDistance(int a[], int b[], int mode)
{
	int i;
	float dis = 0;
	for (i = 0; i < mode; i++)
	{
		if (i >= 3)
			dis += 20 * pow(a[i] - b[i], 2);
		else
			dis += pow(a[i] - b[i], 2);

	}
	return sqrt(dis);
}

float ImageSegmentation::SD(int a[])
{
	float mean = (a[0] + a[1] + a[2]) / 3;
	float variance = pow(mean - a[0], 2) + pow(mean - a[1], 2) + pow(mean - a[2], 2);
	return sqrt(variance / 3);
}

CImg<unsigned char> ImageSegmentation::detectEdge(CImg<unsigned char> img)
{
	CImg<unsigned char> iEdge = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
	int sum = 0;
	cimg_forXY(img, x, y)
	{
		sum = img(x, y, 0) + img(x, y, 1) + img(x, y, 2);
		if (sum < 384)
		{
			int cnt = 0;
			if (x - 1 >= 0 && img(x - 1, y, 0) > 120)
				cnt++;
			if (x + 1 < img.width() && img(x + 1, y, 0) > 120)
				cnt++;
			if (y - 1 >= 0 && img(x, y - 1, 0) > 120)
				cnt++;
			if (y + 1 < img.height() && img(x, y + 1, 0) > 120)
				cnt++;
			if(cnt==1)
				iEdge(x, y) = 255;
		}
		
	}
	iEdge.display();
	return iEdge;
}

CImg<unsigned char> ImageSegmentation::generateHough(CImg<unsigned char> iEdge)
{
	maxn = (int)sqrt(iEdge.width()*iEdge.width() + iEdge.height()*iEdge.height());
	CImg<unsigned char> houghS = CImg<unsigned char>(360, maxn * 2, 1, 1, 0);
	cimg_forXY(iEdge, x, y)
	{
		if (iEdge(x, y) == 255)
		{
			for (int i = 0; i < 360; i++) {
				double theta = (double)i*cimg::PI / 180;
				int p = (int)round(sin(theta) * (double)y + cos(theta) * (double)x);
				if (p < maxn)
					houghS(i, maxn + p)++;
			}
		}
	}
	houghS.display();
	return houghS;
}

void ImageSegmentation::findTopK(CImg<unsigned char> houghS)
{
	unsigned char vote_threshold = 15;
	cimg_forXY(houghS, x, y)
	{
		if (houghS(x, y) > vote_threshold)
		{
			node temp;
			temp.x = x;
			temp.y = y;
			temp.value = houghS(x, y);
			points.push_back(temp);
		}
	}
	sort(points.begin(), points.end());
	vector<node> points2;
	for (int i = points.size() - 1; i > points.size() - 32; i--)
	{
		points2.push_back(points[i]);
	}
	points = points2;
}

void ImageSegmentation::getPoint(CImg<unsigned char> iEdge)
{
	int i, j;
	CImg<unsigned char> I2 = CImg<unsigned char>(iEdge.width(), iEdge.height(), 1, 3, 0);
	cimg_forXY(I2, x, y) {
		I2(x, y, 0) = iEdge(x, y);
		I2(x, y, 1) = iEdge(x, y);
		I2(x, y, 2) = iEdge(x, y);
	}
	ks.clear();
	bs.clear();
	for (i = 0; i < points.size(); i++)
	{
		int flag = 0;
		for (j = 0; j < i; j++)
		{
			//cout << points[i].x << " " << points[i].y << endl;
			if (sqrt((points[i].x - points[j].x)*(points[i].x - points[j].x) + (points[i].y - points[j].y)*(points[i].y - points[j].y)) < 30)
				flag = 1;
		}
		if (flag)
			continue;
		double theta = (double)points[i].x * cimg::PI / 180;
		double k = -1 / tan(theta);
		double b = ((double)points[i].y - maxn) / sin(theta);
		if (isinf(k))
			continue;
		flag = 0;
		for (j = 0; j < ks.size(); j++)
		{
			//cout << points[i].x << " " << points[i].y << endl;
			if ((abs((ks[j] - k) / k)<0.01 || abs(thetas[j] - theta)<5) && (abs((bs[j] - b) / b)<0.01 || abs((bs[j] / ks[j] - b / k) / (b / k)) <0.05))
				flag = 1;
		}
		if (flag)
			continue;
		ks.push_back(k);
		bs.push_back(b);
		thetas.push_back(theta);
		cout << "Line " << i << ": y = " << k << "x + " << b << endl;

		/*const int x0 = (double)(-b) / k;
		const int x1 = (double)(iEdge.height() - b) / k;
		const int y0 = b;
		const int y1 = iEdge.width() * k + b;
		const double blue[] = { 0, 0 , 255 };
		if (abs(k) > 1)
			I2.draw_line(x0, 0, x1, iEdge.height(), blue);
		else
			I2.draw_line(0, y0, iEdge.width(), y1, blue);*/
	}
	points3.clear();
	for (int i = 0; i < ks.size(); i++) {
		const double red[] = { 255, 0 , 0 };
		for (int j = i + 1; j < ks.size(); j++) {
			double k0 = ks[i];
			double k1 = ks[j];
			double b0 = bs[i];
			double b1 = bs[j];

			double x = (b1 - b0) / (k0 - k1);
			double y = (k0*b1 - k1 * b0) / (k0 - k1);
			const double yellow[] = { 255, 255 , 0 };
			if (x >= 0 && x < I2.width() && y >= 0 && y < I2.height()) {
				cout << "Intersection " << ": x = " << x << ", y = " << y << endl;
				I2.draw_circle(x, y, 10, yellow);
				node temp;
				temp.x = int(x);
				temp.y = int(y);
				temp.value = temp.x + temp.y;
				points3.push_back(temp);
			}
		}
	}
	I2.display();
}

ImageSegmentation::Homography ImageSegmentation::getHomography() {
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
	int ext = 5;
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


CImg<unsigned char> ImageSegmentation::getImage(Homography h, CImg<unsigned char> img)
{
	CImg<unsigned char> I2 = CImg<unsigned char>(210 * ratio, 299 * ratio, 1, 3, 0);
	cimg_forXY(I2, x, y) {
		int x1 = (h.c1* x + h.c2 * y + h.c3 * x * y + h.c4) ;
		int y1 = (h.c5* x + h.c6 * y + h.c7 * x * y + h.c8) ;
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









