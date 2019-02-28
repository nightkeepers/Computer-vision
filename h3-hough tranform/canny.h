//
//  canny.h
//  Canny Edge Detector
//
//  Created by Hasan Akgün on 21/03/14.
//  Copyright (c) 2014 Hasan Akgün. All rights reserved.
//

#ifndef _CANNY_
#define _CANNY_
#include "CImg.h"
#include <vector>

using namespace std;
using namespace cimg_library;

class canny {
private:
	CImg<unsigned char> img; //Original Image
	CImg<unsigned char> grayscaled; // Grayscale
	CImg<unsigned char> gFiltered; // Gradient
	CImg<unsigned char> sFiltered; //Sobel Filtered
	CImg<unsigned char> angles; //Angle Map
	CImg<unsigned char> non; // Non-maxima supp.
	CImg<unsigned char> thres; //Double threshold and final
	CImg<unsigned char> final;
public:

	canny(char filename[], int Gauss = 3, double sigma = 1, int low = 20, int high = 40); //Constructor
	CImg<unsigned char> toGrayScale();
	vector<vector<double>> createFilter(int, int, double); //Creates a gaussian filter
	CImg<unsigned char> useFilter(CImg<unsigned char>, vector<vector<double>>); //Use some filter
	CImg<unsigned char> sobel(); //Sobel filtering
	CImg<unsigned char> nonMaxSupp(); //Non-maxima supp.
	CImg<unsigned char> threshold(CImg<unsigned char>, int, int); //Double threshold and finalize picture
	CImg<unsigned char> connect(CImg<unsigned char>);
};

#endif
