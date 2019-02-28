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
#include <string>
#include <sstream>
#include "Final.h"

using namespace cimg_library;
using namespace std;

int main()
{
	int i;
	char path[2][30] = { "ImageData/ImageData/0.bmp" };
	for (i = 0; i < 1; i++)
	{
		stringstream ss;
		ss << i;
		string s = "ImageData/ImageData/";
		s += ss.str();
		s += ".bmp";
		cout << s << endl;

		Final temp(s, i);
	}


    return 0;
}

