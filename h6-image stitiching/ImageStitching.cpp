#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "ImageStitching.h"


using namespace std;
using namespace cimg_library;

ImageStitching::ImageStitching(vector<string> filenames) {
	int i;
	CImg<unsigned char> image;
	thresold = 0.8;
	thresold2 = 250;
	int cnt = 9;
	for (i = 0; i < filenames.size(); i++)
	{
		image.load_bmp(filenames[i].c_str());
		int w = image.width();
		int h = image.height();
		image.resize(w * 0.5, h * 0.5);
		//image.rotate(270);
		//image.display();
		images.push_back(image);
	}
	while (images.size() > 1)
	{
		//thresold = thresold - 0.06*cnt;
		stitched.clear();
		for (i = 0; i < images.size() / 2; i++)
		{
			CImg<unsigned char> tmp = siftImage(images[2 * i], images[2 * i + 1]);
			if (images.size() % 2 == 1 && images.size() - 1 == 2 * i + 2)
				tmp = siftImage(tmp, images[2 * i + 2]);
			stitched.push_back(tmp);
			stringstream ss;
			ss << cnt;
			cnt++;
			string temp = "result/result" + ss.str() + ".bmp";
			//tmp.save(temp.c_str());
		}
		images = stitched;
	}
	CImg<unsigned char> result = stitched[0];
	result.display();
	result.save("result/result1.bmp");
}

CImg<unsigned char> ImageStitching::toGrayScale(CImg<unsigned char> img)
{
	CImg<unsigned char> grayscaled = CImg<unsigned char> (img.width(), img.height(), 1, 1); //To one channel
	cimg_forXY(img, x, y) {
		int r = img(x, y, 0);
		int g = img(x, y, 1);
		int b = img(x, y, 2);

		double newValue = (r * 0.2126 + g * 0.7152 + b * 0.0722);
		grayscaled(x, y) = newValue;
	}
	return grayscaled;
}


CImg<unsigned char> ImageStitching::BlendImage(CImg<unsigned char> img, CImg<unsigned char> img2, float shiftX, float shiftY)
{
	int x, y;
	CImg<unsigned char> newIm(img.width() + abs(shiftX), img.height() + abs(shiftY), 1, 3, 0);
	int sX = max(-shiftX, 0.0f);
	int sY = max(-shiftY, 0.0f);
	cimg_forXY(img, x, y) {
		int tempx = sX + x;
		int tempy = sY + y;
		if (tempx < 0)
			tempx = 0;
		if (tempx >= newIm.width())
			tempx = newIm.width() - 1;
		if (tempy < 0)
			tempy = 0;
		if (tempy >= newIm.height())
			tempy = newIm.height() - 1;
		newIm(tempx, tempy, 0) = img(x, y, 0);
		newIm(tempx, tempy, 1) = img(x, y, 1);
		newIm(tempx, tempy, 2) = img(x, y, 2);
	}
	sX = max(shiftX, 0.0f);
	sY = max(shiftY, 0.0f);
	cimg_forXY(img2, x, y) {
		int tempx = sX + x;
		int tempy = sY + y;
		if (tempx < 0)
			tempx = 0;
		if (tempx >= newIm.width())
			tempx = newIm.width() - 1;
		if (tempy < 0)
			tempy = 0;
		if (tempy >= newIm.height())
			tempy = newIm.height() - 1;
		newIm(tempx, tempy, 0) = img2(x, y, 0);
		newIm(tempx, tempy, 1) = img2(x, y, 1);
		newIm(tempx, tempy, 2) = img2(x, y, 2);
	}
	return newIm;
}

float ImageStitching::computeDistance(SiftDescriptor SD1, SiftDescriptor SD2)
{
	int i;
	float dis = 0;
	for (i = 0; i < 128; i++) {
		dis += pow(SD1.descriptor[i] - SD2.descriptor[i], 2);
	}
	return sqrt(dis);
}

float ImageStitching::getShift(vector<float> shiftV)
{
	int i, j, ans = 0;
	float cnt, maxcnt = 0;
	for (i = 0; i < shiftV.size(); i++)
	{
		cnt = 0;
		for (j = 0; j < shiftV.size(); j++)
		{
			if (abs(shiftV[i] - shiftV[j]) <= 3)
				cnt++;
			if (maxcnt < cnt)
			{
				maxcnt = cnt;
				ans = shiftV[i];
			}
		}
	}
	return ans;
}

CImg<unsigned char> ImageStitching::siftImage(CImg<unsigned char> img, CImg<unsigned char> img2)
{
	int i, j, dis;
	unsigned char red[3] = { 255, 0, 0 };
	unsigned char yellow[3] = { 255, 255, 0 };
	CImg<unsigned> gray1 = toGrayScale(img);
	CImg<unsigned> gray2 = toGrayScale(img2);
	vector<SiftDescriptor> VSD1 = Sift::compute_sift(gray1);
	vector<SiftDescriptor> VSD2 = Sift::compute_sift(gray2);
	vector<float> shiftx, shifty;
	
	CImg<unsigned char> newIm(img.width() + img2.width(), max(img.height(), img2.height()), 1, 3, 0);
	cimg_forXY(img, x, y) {
		newIm(x, y, 0) = img(x, y, 0);
		newIm(x, y, 1) = img(x, y, 1);
		newIm(x, y, 2) = img(x, y, 2);
	}
	cimg_forXY(img2, x, y) {
		newIm(x + img.width(), y, 0 ) = img2(x, y, 0);
		newIm(x + img.width(), y, 1) = img2(x, y, 1);
		newIm(x + img.width(), y, 2) = img2(x, y, 2);
	}

	int bestx[2], besty[2];
	float bestd[2];
	//newIm.display();

	cout << VSD1.size()<<" "<<VSD2.size() << endl;
	for (i = 0; i < VSD1.size(); i++)
	{
		if (i % 100 == 0)
		{
			cout << i << "echo" << endl;
		}
		bestd[0] = 100000;
		bestd[1] = 100000;
		for (j = 0; j < VSD2.size(); j++)
		{
			dis = computeDistance(VSD1[i],VSD2[j]);
			if (dis < bestd[0])
			{
				bestd[1] = bestd[0];
				bestx[1] = bestx[0];
				besty[1] = besty[0];
				bestd[0] = dis;
				bestx[0] = i;
				besty[0] = j;
				continue;
			}
			if (dis < bestd[1])
			{
				bestd[1] = dis;
				bestx[1] = i;
				besty[1] = j;
			}

		}
		float ratio = bestd[0] / bestd[1];
		if (bestd[0] < 200) {
			thresold = 0.6;
		}
		else
			thresold = 0.4;
		if (ratio < thresold)
		{
			newIm.draw_circle(VSD1[bestx[0]].col, VSD1[bestx[0]].row, 1, red, 1.0f, 1);
			newIm.draw_circle(VSD2[besty[0]].col + img.width(), VSD2[besty[0]].row, 1, yellow, 0.5f, 1);
			newIm.draw_line(VSD1[bestx[0]].col, VSD1[bestx[0]].row, VSD2[besty[0]].col + img.width(), VSD2[besty[0]].row, red);
			float temp1 = VSD1[bestx[0]].col - VSD2[besty[0]].col;
			float temp2 = VSD1[bestx[0]].row - VSD2[besty[0]].row;
			shiftx.push_back(temp1);
			shifty.push_back(temp2);

		}
	}
	newIm.display();
	float shiftX = getShift(shiftx);
	float shiftY = getShift(shifty);

	//newIm.display();
	CImg<unsigned char> result = BlendImage(img, img2, shiftX, shiftY);
	return result;
}











