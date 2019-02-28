#include <iostream>
#include <cstring>
#include "CImg.h"
#include "histogram.h"


using namespace std;
using namespace cimg_library;



Histogram::Histogram(char filename[]) {
	img.load_bmp(filename);
	CImg<unsigned char> grayscaled = toGrayScale(img);
	equalization(grayscaled);
	equalization(img);

}

CImg<unsigned char> Histogram::toGrayScale(CImg<unsigned char> src)
{
	CImg<unsigned char> grayscaled= CImg<unsigned char>(src.width(), src.height(), 3); //To one channel
	cimg_forXY(img, x, y) {
		int r = src(x, y, 0);
		int g = src(x, y, 1);
		int b = src(x, y, 2);

		double newValue = (r * 0.2126 + g * 0.7152 + b * 0.0722);
		grayscaled(x, y, 0) = newValue;
		grayscaled(x, y, 1) = newValue;
		grayscaled(x, y, 2) = newValue;
	}
	return grayscaled;
}

CImg<unsigned char> Histogram::equalization(CImg<unsigned char> src1)
{
	int i, j;
	int num[256];//number of pixels
	float proba[256];//probablity
	float accu[256];//accumulative
	CImg<unsigned char> src = src1;
	src.display();
	for (i = 0; i < 3; i++)
	{
		memset(num, 0, sizeof(num));
		memset(proba, 0, sizeof(proba));
		memset(accu, 0, sizeof(accu));
		cimg_forXY(src, x, y)
		{
			num[src(x, y, i)]++;
		}

		for (j = 0; j < 256; j++)
			proba[j] = (float)num[j]/ (src.width()*src.height());

		accu[0] = proba[0];
		for (j = 1; j < 256; j++)
			accu[j] = accu[j - 1] + proba[j];

		cimg_forXY(src, x, y)
		{
			src(x, y, i) = int(accu[src(x, y, i)] * (255 - 0));
		}

	}
	src.display();
	return src;

}

void Histogram::setImg(char filename1[], char filename2[])
{
	src.load_bmp(filename1);
	target.load_bmp(filename2);
}

CImg<float> Histogram::transfer()
{
	int i;
	CImg<float> ans = src, target2 = target;
	float sum1[3], sum2[3], mean1[3], mean2[3], sd1[3], sd2[3];
	memset(sum1, 0, sizeof(sum1));
	memset(sum2, 0, sizeof(sum2));
	memset(sd1, 0, sizeof(sd1));
	memset(sd2, 0, sizeof(sd2));
	src.display();
	target.display();
	cimg_forXY(src, x, y)
	{
		//rgb to xyz to lms
		float r = src(x, y, 0);
		float g = src(x, y, 1);
		float b = src(x, y, 2);
		float L = 0.3811 * r + 0.5783 * g + 0.0402 * b;
		float M = 0.1967 * r + 0.7244 * g + 0.0782 * b;
		float S = 0.0241 * r + 0.1288 * g + 0.8444 * b;

		if (L == 0)
			L = 0.5;
		if (M == 0)
			M = 0.5;
		if (S == 0)
			S = 0.5;

		L = log(L);
		M = log(M);
		S = log(S);

		//lms to lab

		float a1 = (1.0 / sqrt(3));
		float a2 = a1;
		float a3 = a1;
		float b1 = (1.0 / sqrt(6)), b2 = b1, b3 = -2 * b1;
		float c1 = (1.0 / sqrt(2)), c2 = - c1, c3 = 0;

		float l = a1 * L + a2 * M + a3 * S;
		float alpha = b1 * L + b2 * M + b3 * S;
		float beta = c1 * L + c2 * M + c3 * S;

		sum1[0] += l;
		sum1[1] += alpha;
		sum1[2] += beta;

		ans(x, y, 0) = l;
		ans(x, y, 1) = alpha;
		ans(x, y, 2) = beta;
	}

	for (i = 0; i < 3; i++)
	{
		mean1[i] = sum1[i] / (src.width() * src.height());
		cimg_forXY(ans, x, y) {
			sd1[i] += (ans(x, y, i) - mean1[i]) * (ans(x, y, i) - mean1[i]);
		}
		sd1[i] = sqrt(sd1[i]) / (src.width() * src.height());
	}

	cimg_forXY(target, x, y)
	{
		//rgb to xyz to lms
		float r = target(x, y, 0);
		float g = target(x, y, 1);
		float b = target(x, y, 2);
		float L = 0.3811 * r + 0.5783 * g + 0.0402 * b;
		float M = 0.1967 * r + 0.7244 * g + 0.0782 * b;
		float S = 0.0241 * r + 0.1288 * g + 0.8444 * b;

		if (L == 0)
			L = 0.5;
		if (M == 0)
			M = 0.5;
		if (S == 0)
			S = 0.5;

		L = log(L);
		M = log(M);
		S = log(S);

		//lms to lab

		float a1 = (1.0 / sqrt(3));
		float a2 = a1;
		float a3 = a1;
		float b1 = (1.0 / sqrt(6)), b2 = b1, b3 = -2 * b1;
		float c1 = (1.0 / sqrt(2)), c2 = -c1, c3 = 0;

		float l = a1 * L + a2 * M + a3 * S;
		float alpha = b1 * L + b2 * M + b3 * S;
		float beta = c1 * L + c2 * M + c3 * S;

		sum2[0] += l;
		sum2[1] += alpha;
		sum2[2] += beta;

		target2(x, y, 0) = l;
		target2(x, y, 1) = alpha;
		target2(x, y, 2) = beta;
	}
	for (i = 0; i < 3; i++)
	{
		mean2[i] = sum2[i] / (target.width() * target.height());
		cimg_forXY(target2, x, y) {
			sd2[i] += (target2(x, y, i) - mean2[i]) * (target2(x, y, i) - mean2[i]);
		}
		sd2[i] = sqrt(sd2[i]) / (target.width() * target.height());
	}

	for (i = 0; i < 3; i++)
	{
		cimg_forXY(ans, x, y) {
			ans(x, y, i) = (ans(x, y, i) - mean1[i]) * sd2[i] / sd1[i] + mean2[i];
		}
	}

	//lab ro rgb
	cimg_forXY(ans, x, y) {
		float L = sqrt(3) / 3 * ans(x, y, 0) + sqrt(6) / 6 * ans(x, y, 1) + sqrt(2) / 2 * ans(x, y, 2);
		float M = sqrt(3) / 3 * ans(x, y, 0) + sqrt(6) / 6 * ans(x, y, 1) - sqrt(2) / 2 * ans(x, y, 2);
		float S = sqrt(3) / 3 * ans(x, y, 0) - sqrt(6) / 3 * ans(x, y, 1);
		L = pow(2.71828, L);
		M = pow(2.71828, M);
		S = pow(2.71828, S);
		ans(x, y, 0) = 4.4679 * L - 3.5873 * M + 0.1193 * S;
		ans(x, y, 1) = -1.2186 * L + 2.3809 * M - 0.1624 * S;
		ans(x, y, 2) = 0.0497 * L - 0.2439 * M + 1.2045 * S;
	}
	ans.display();
	return ans;
}


