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
#include "ImageMorphing.h"

using namespace cimg_library;
using namespace std;

int main()
{
	int i;
	char path[2][20] = { "1.bmp", "3.bmp"  };
	ImageMorphing temp(path[0],path[1]);
    return 0;
}

