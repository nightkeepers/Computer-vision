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
#include "canny.h"
#include "CImg.h"
#include "hough.h"

using namespace cimg_library;
using namespace std;

int main()
{
	int i;
	char path[2][20] = { "5.bmp","3.bmp" };
	//Hough temp(path[1], 0);//line mode
	Hough temp(path[0], 1);//circle mode
	//canny temp2(path[0]);

    return 0;
}

