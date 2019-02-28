//
//  main.cpp
//  Canny Edge Detector
//
//  Created by Hasan Akgün on 21/03/14.
//  Copyright (c) 2014 Hasan Akgün. All rights reserved.
//

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include "CImg.h"
#include "histogram.h"

using namespace cimg_library;
using namespace std;

int main()
{
	int i;
	char path[11][20] = { "1.bmp", "2.bmp" ,"3.bmp", "4.bmp", "5.bmp","6.bmp" ,"7.bmp","8.bmp", "9.bmp","10.bmp","11.bmp" };
	for (i = 0; i < 5; i++)
	{
		Histogram temp(path[i]);
	}
	//canny temp2(path[0]);
	Histogram temp;
	temp.setImg(path[0], path[4]);
	temp.transfer();
	temp.setImg(path[5], path[6]);
	temp.transfer();
	temp.setImg(path[7], path[8]);
	temp.transfer();
	temp.setImg(path[9], path[10]);
	temp.transfer();
	temp.setImg(path[1], path[2]);
	temp.transfer();
    return 0;
}

