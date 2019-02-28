#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>
#include "CImg.h"
#include "hough.h"

using namespace std;
using namespace cimg_library;


/*CImg<unsigned char> Hough::edgeDetect(CImg<unsigned char> src) {
	int sobelX[3][3] = { { -1,0,1 },{ -2,0,2 },{ -1,0,1 } };
	int sobelY[3][3] = { { 1,2,1 },{ 0,0,0 },{ -1,-2,-1 } };
	int width = src.width();
	int height = src.height();
	CImg<unsigned char> G(width, height, 1, 3);
	CImg<unsigned char> newImg(width, height, 1, 3);
	newImg.fill(0);
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			double sumX = 0.0;
			double sumY = 0.0;
			for (int row = 0; row < 3; row++) {
				for (int col = 0; col < 3; col++) {
					sumX += sobelX[row][col] * src(i - 1 + row, j - 1 + col, 0);
					sumY += sobelY[row][col] * src(i - 1 + row, j - 1 + col, 0);
				}
			}
			
			newImg(i, j, 0) = sqrt(sumX*sumX + sumY * sumY) > 85 ? 255 : 0;
			newImg(i, j, 1) = newImg(i, j, 0);
			newImg(i, j, 2) = newImg(i, j, 0);
		}
		if (i % 100 == 0)
			cout << i << endl;
	}
	newImg.display();
	//newImg.dilate(3.5);
	return newImg;
}*/

Hough::Hough() {
	/*img.load_bmp(filename);
	edgeDetect(img);
	if (mode == 0)
	{
		CImg<unsigned char> temp = generateHough();
		findTopK();	//get voting result
		temp = drawLine();
		drawIntersections();
		drawEdge();
	}
	else
	{
		houghCircles(iEdge, 80, 200);
	}*/
}

CImg<unsigned char> Hough::toGrayScale(CImg<unsigned char> src)
{
	CImg<unsigned char> grayscaled= CImg<unsigned char>(src.width(), src.height()); //To one channel
	cimg_forXY(src, x, y) {
		int r = src(x, y, 0);
		int g = src(x, y, 1);
		int b = src(x, y, 2);

		double newValue = (r * 0.2126 + g * 0.7152 + b * 0.0722);
		grayscaled(x, y) = newValue;
	}
	return grayscaled;
}

CImg<unsigned char> Hough::sobel(CImg<unsigned char> src) {
	int sobelX[3][3] = { { -1,0,1 },{ -2,0,2 },{ -1,0,1 } };
	int sobelY[3][3] = { { 1,2,1 },{ 0,0,0 },{ -1,-2,-1 } };
	int width = src.width();
	int height = src.height();
	CImg<unsigned char> newImg(width - 2, height - 2);
	newImg.fill(0);
	angles = CImg<unsigned char>(width - 2, height - 2);
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			double sumX = 0.0;
			double sumY = 0.0;
			for (int row = 0; row < 3; row++) {
				for (int col = 0; col < 3; col++) {
					sumX += sobelX[row][col] * src(i - 1 + row, j - 1 + col);
					sumY += sobelY[row][col] * src(i - 1 + row, j - 1 + col);
				}
			}

			double sq2 = sqrt(sumX * sumX + sumY * sumY);
			if (sq2 > 255) //Unsigned Char Fix
				sq2 = 255;
			newImg(i - 1, j - 1) = sq2;

			if (sumX == 0) //Arctan Fix
				angles(i - 1, j - 1) = 90;
			else
				angles(i - 1, j - 1) = atan(sumY / sumX);

		}
		if (i % 100 == 0)
			cout << i << endl;
	}
	//newImg.display();
	//newImg.dilate(3.5);
	return newImg;
}

CImg<unsigned char> Hough::nonMaxSupp(CImg<unsigned char> sFiltered)
{
	CImg<unsigned char> nonMaxSupped = CImg<unsigned char>(sFiltered.width(), sFiltered.height(), 1, 1, 0);
	for (int i = 1; i< sFiltered.width() - 1; i++) {
		for (int j = 1; j<sFiltered.height() - 1; j++) {
			float Tangent = angles(i, j);//??????????

			nonMaxSupped(i - 1, j - 1) = sFiltered(i, j);
			//Horizontal Edge
			if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
			{
				if ((sFiltered(i, j) < sFiltered(i, j + 1)) || (sFiltered(i, j) < sFiltered(i, j - 1)))
					nonMaxSupped(i - 1, j - 1) = 0;
			}
			//Vertical Edge
			if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
			{
				if ((sFiltered(i, j) < sFiltered(i + 1, j)) || (sFiltered(i, j) < sFiltered(i - 1, j)))
					nonMaxSupped(i - 1, j - 1) = 0;
			}

			//-45 Degree Edge
			if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
			{
				if ((sFiltered(i, j) < sFiltered(i - 1, j + 1)) || (sFiltered(i, j) < sFiltered(i + 1, j - 1)))
					nonMaxSupped(i - 1, j - 1) = 0;
			}

			//45 Degree Edge
			if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
			{
				if ((sFiltered(i, j) < sFiltered(i + 1, j + 1)) || (sFiltered(i, j) < sFiltered(i - 1, j - 1)))
					nonMaxSupped(i - 1, j - 1) = 0;
			}
		}
	}
	return nonMaxSupped;
}

CImg<unsigned char> Hough::threshold(CImg<unsigned char> imgin, int low, int high)
{
	if (low > 255)
		low = 255;
	if (high > 255)
		high = 255;

	CImg<unsigned char> EdgeMat = CImg<unsigned char>(imgin.width(), imgin.height(), 1, 1, 0);

	for (int i = 0; i<imgin.width(); i++)
	{
		for (int j = 0; j<imgin.height(); j++)
		{
			EdgeMat(i, j) = imgin(i, j);
			if (EdgeMat(i, j) > high)
				EdgeMat(i, j) = 255;
			else if (EdgeMat(i, j) < low)
				EdgeMat(i, j) = 0;
			else
			{
				bool anyHigh = false;
				bool anyBetween = false;
				for (int x = i - 1; x < i + 2; x++)
				{
					for (int y = j - 1; y<j + 2; y++)
					{
						if (x <= 0 || y <= 0 || x >= EdgeMat.width() || y >= EdgeMat.height()) //Out of bounds
							continue;
						else
						{
							if (EdgeMat(x, y) > high)
							{
								EdgeMat(i, j) = 255;
								anyHigh = true;
								break;
							}
							else if (EdgeMat(x, y) <= high && EdgeMat(x, y) >= low)
								anyBetween = true;
						}
					}
					if (anyHigh)
						break;
				}
				if (!anyHigh && anyBetween)
					for (int x = i - 2; x < i + 3; x++)
					{
						for (int y = j - 1; y<j + 3; y++)
						{
							if (x < 0 || y < 0 || x >= EdgeMat.width() || y >= EdgeMat.height()) //Out of bounds
								continue;
							else
							{
								if (EdgeMat(x, y) > high)
								{
									EdgeMat(i, j) = 255;
									anyHigh = true;
									break;
								}
							}
						}
						if (anyHigh)
							break;
					}
				if (!anyHigh)
					EdgeMat(i, j) = 0;
			}
		}
	}
	return EdgeMat;
}


CImg<unsigned char> Hough::connect(CImg<unsigned char> img)
{
	CImg<unsigned char> EdgeMat = img;
	CImg<unsigned char> vis = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
	/*for (int i = 0; i < img.width(); i++)
	{
		for (int j = 0; j < img.height(); j++)
		{
			if (i<4 || j<4 || j>img.height() - 5 || i>img.width() - 5)
			{
				EdgeMat(i, j) = img(i, j);
				vis(i, j) = 0;
				continue;
			}
			if (img(i, j) == 255 && EdgeMat(i, j) != 255)
			{
				for (int a = -2; a < 3; a++)
				{
					for (int b = -2; b < 3; b++)
					{
						if (abs(a) < 2 && abs(b) < 2)
							continue;
						else if (img(i + a, j + b) == 255 && img(i - a, j - b) == 255 && img(i - a / 2, j - b / 2) == 255 && img(i + a + a / 2, j + b + b / 2) == 255)
						{
							if (EdgeMat(ceil(a / 2) + i, j + ceil(b / 2) != 255))
							{
								EdgeMat(floor(a / 2) + i, j + floor(b / 2)) = 255;
							}
						}
					}
				}
			}
			EdgeMat(i, j) = img(i, j);
			vis(i, j) = 0;
		}
	}*/
	for (int i = 0; i < img.width(); i++)
	{
		for (int j = 0; j < img.height(); j++)
		{
			if (EdgeMat(i, j) == 255 && vis(i, j) != 1)
			{
				queue<node> bfs;
				queue<node> edge;
				node temp;
				temp.x = i;
				temp.y = j;
				temp.value = 0;
				bfs.push(temp);
				edge.push(temp);
				while (!bfs.empty())
				{
					temp = bfs.front();
					bfs.pop();
					//if (temp.value >= 5)
						//break;
					for (int a = -1; a < 2; a++)
					{
						for (int b = -1; b < 2; b++)
						{
							if (a == 0 && b == 0)
								continue;
							if (temp.x + a < 0 || temp.y + b < 0 || temp.x + a >= img.width() || temp.y + b >= img.height())
								continue;
							if (vis(temp.x + a, temp.y + b) == 0 && EdgeMat(temp.x + a, temp.y + b) == 255)
							{
								vis(temp.x + a, temp.y + b) = 1;
								node temp2;
								temp2.x = temp.x + a;
								temp2.y = temp.y + b;
								temp2.value = temp.value + 1;
								bfs.push(temp2);
								edge.push(temp2);
							}
						}
					}
				}
				if (temp.value < 20)
				{
					while (!edge.empty())
					{
						temp = edge.front();
						edge.pop();
						EdgeMat(temp.x, temp.y) = 0;
					}
				}
			}
		}
	}
	return EdgeMat;
}

CImg<unsigned char> Hough::edgeDetect(CImg<unsigned char> src){
	CImg<unsigned char> grayscaled = toGrayScale(src);
	//grayscaled.display();
	CImg<unsigned char> blur = grayscaled.blur(1);
	//blur.display();
	CImg<unsigned char> sobleimg = sobel(blur);
	//sobleimg.display("sobleimg");
	CImg<unsigned char> non = nonMaxSupp(sobleimg);
	//non.display("nonMaxSupp");
	CImg<unsigned char> iEdge2 = threshold(non, 90, 110);
	//iEdge2.display();
	iEdge = connect(iEdge2);
	//iEdge.display("Iedge");
	return iEdge;
}


CImg<unsigned char> Hough::generateHough()
{
	maxn = (int)sqrt(iEdge.width()*iEdge.width() + iEdge.height()*iEdge.height());
	houghS = CImg<unsigned char>(360, maxn * 2, 1, 1, 0);
	cimg_forXY(iEdge, x, y)
	{
		if (iEdge(x, y) == 255)
		{
			for (int i = 0; i < 360; i++) {
				double theta = (double)i*cimg::PI/ 180;
				int p = (int)round(sin(theta) * (double)y + cos(theta) * (double)x);
				if ( p < maxn)
					houghS(i, maxn + p)++;
			}
		}
	}
	//houghS.display();
	return houghS;
}

void Hough::findTopK()
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
	for (int i = points.size() - 1; i > points.size() - 152; i--)
	{
		points2.push_back(points[i]);
	}
	points = points2;
}

CImg<unsigned char> Hough::drawLine()
{
	int i, j, cnt = 0;

	I2 = CImg<unsigned char>(iEdge.width(), iEdge.height(), 1, 3, 0);
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
			if (sqrt((points[i].x - points[j].x)*(points[i].x - points[j].x) + (points[i].y - points[j].y)*(points[i].y - points[j].y)) < 60)
				flag = 1;
		}
		if (flag||i==0)
			continue;
		double theta = (double)points[i].x * cimg::PI / 180;
		double k = -1/tan(theta);
		double b = ((double)points[i].y - maxn) / sin(theta);
		if (isinf(k))
			continue;
		flag = 0;
		for (j = 0; j < ks.size(); j++)
		{
			/*if (i == 11||i==10)
				cout << points[i].x << " " << points[i].y << endl;*/
			double bias = abs(thetas[j] - points[i].x);
			if (abs((ks[j] - k) / k) < 0.01 || bias < 5 || bias > 355 || (bias > 175 && bias < 185))
			{
				if(abs((bs[j] - b) / sqrt(k*k + 1)) < 50 || abs((bs[j] / ks[j] - b / k)*k / sqrt(k*k + 1)) < 50)
					flag = 1;
			}
		}
		if (flag)
			continue;
		ks.push_back(k);
		bs.push_back(b);
		thetas.push_back(points[i].x);
		cout << "Line " << i << ": y = " << k << "x + " << b << endl;

		const int x0 = (double)(-b)/k;
		const int x1 = (double)(iEdge.height()-b)/k;
		const int y0 = b;
		const int y1 = iEdge.width() * k + b;
		const double blue[] = { 0, 0 , 255};
		if (abs(k) > 1)
			I2.draw_line(x0, 0, x1, iEdge.height(), blue);
		else
			I2.draw_line(0,y0, iEdge.width(), y1, blue);
		cnt++;
		if (cnt >= 4)
			break;
	}
	//I2.display("I2");
	return I2;
}

void Hough::drawIntersections() {
	int i, j;
	I3 = I2;
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
				I3.draw_circle(x, y, 10, yellow);
				node temp;
				temp.x = int(x);
				temp.y = int(y);
				temp.value = temp.x + temp.y;
				points3.push_back(temp);
			}
		}
	}
	//I3.display("I3");
}

void Hough::drawEdge() {
	I4 = I3;
	int i, j;
	for (int i = 0; i < ks.size(); i++) {
		const double red[] = { 255, 0 , 0 };
		xs.clear();
		ys.clear();
		for (int j =0; j < ks.size(); j++) {
			if (i == j)
				continue;
			double k0 = ks[i];
			double k1 = ks[j];
			double b0 = bs[i];
			double b1 = bs[j];

			double x = (b1 - b0) / (k0 - k1);
			double y = (k0*b1 - k1 * b0) / (k0 - k1);
			if (x >= 0 && x < I2.width() && y >= 0 && y < I2.height()) {
				xs.push_back(x);
				ys.push_back(y);
			}
		}
		if(xs.size()>=2)
			I4.draw_line(xs[0],ys[0],xs[1],ys[1],red);
	}
	I4.display("I4");
}

void Hough::houghCircles(CImg<unsigned char>& img, int minR, int maxR)
{
	int i, j, bestR = 0;
	I2 = CImg<unsigned char>(iEdge.width(), iEdge.height(), 1, 3, 0);

	cimg_forXY(I2, x, y) {
		I2(x, y, 0) = iEdge(x, y);
		I2(x, y, 1) = iEdge(x, y);
		I2(x, y, 2) = iEdge(x, y);
	}
	for (i = minR; i < maxR; i += 10) {
		houghS2 = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
		cimg_forXY(img, x, y) {
			if (img(x, y) == 255)
			{
				for (j = 0; j < 360 ; j++)
				{
					int x0 = x - i * cos(cimg::PI * j / 180);
					int y0 = y - i * sin(cimg::PI * j / 180);
					if (x0 > 0 && x0 < img.width() && y0 > 0 && y0 < img.height()) {
						houghS2(x0, y0)++;
					}
				}
			}
		}

		for (int x = 0; x < houghS2.width(); x++) {
			for (int y = 0; y <  houghS2.height(); y++) {
				bestR = max(bestR, (int)houghS2(x,y));

			}

		}

		node temp;
		temp.x = i;
		temp.value = bestR;
		nodes.push_back(temp);
		cout << i << endl;
	}
	sort(nodes.begin(), nodes.end());
	int size1 = nodes.size() - 1;
	for (int i = 0; i < 4; i++) {
		houghS2 = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);
		cimg_forXY(img, x, y) {
			if (img(x, y) != 0)
			{
				for (j = 0; j < 360; j++)
				{

					int x0 = x - nodes[size1 - i].x * cos(cimg::PI * j / 180);
					int y0 = y - nodes[size1 - i].x * sin(cimg::PI * j / 180);
					if (x0 > 0 && x0 < img.width() && y0 > 0 && y0 < img.height()) {
						houghS2(x0, y0)++;
					}
				}
			}
			if (x % 100 == 0&&y==0)
				cout << x << endl;
		}
		cimg_forXY(houghS2, x, y) {
			if (houghS2(x, y) != 0) {
				node temp;
				temp.x = x;
				temp.y = y;
				temp.value = houghS2(x, y);
				circles.push_back(temp);
			}
		}
		drawCircle(nodes[size1 - i].x);

	}
	I2.display();
}


void Hough::drawCircle(int r) {
	int i, cnt = 0, size1 = circles.size() - 1, cnt2 = 0;
	unsigned char red[3] = { 255, 0, 0 };

	sort(circles.begin(), circles.end()); 


	while (1) {
		int a = circles[size1 - cnt].x, b = circles[size1 - cnt].y;
		cnt++;
		for (i = 0; i < center.size(); i++) {
			if (sqrt(pow((center[i].x - a), 2) + pow((center[i].y - b), 2)) < r + center[i].value) {
				break; 
			}
		}
		if (i == center.size()) {
			node temp;
			temp.x = a;
			temp.y = b;
			temp.value = r;
			center.push_back(temp);
			cnt2++;
			//cout << cnt2 << endl;
			I2.draw_circle(a, b, r, red, 5.0f, 1);
			cout << a << " " << b << " " << r << endl;
			break;
		}
	}
}