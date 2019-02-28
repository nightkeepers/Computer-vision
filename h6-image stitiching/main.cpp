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
#include "ImageStitching.h"

using namespace cimg_library;
using namespace std;

int main()
{
	int i;
	char path[2][20] = { "1.bmp", "3.bmp"  };
	vector<string> filenames;
	for (i = 0; i < 2; i++)
	{
		string temp;
		stringstream ss;
		ss << i;
		temp = "image1/" + ss.str() + ".bmp";
		cout << temp << endl;
		filenames.push_back(temp);
	}
	ImageStitching t(filenames);
    return 0;
}

