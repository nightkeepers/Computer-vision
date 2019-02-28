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

using namespace cimg_library;
using namespace std;

int main()
{
	int i;
	char path[4][20] = { "bigben.bmp", "lena.bmp", "stpietro.bmp", "twows.bmp" }; 
	for (i = 0; i < 0; i++)
	{
		canny temp(path[i]);
	}
	//canny temp(path[1],3);
	//canny temp2(path[1], 5);
	//canny temp3(path[1], 7);

	//canny temp4(path[1], 3, 1);
	//canny temp5(path[1], 3, 2);
	//canny temp6(path[1], 3, 8);

	//canny temp7(path[1], 3, 1, 5);
	//canny temp8(path[1],3, 1, 20);
	//canny temp9(path[1], 3, 1, 60, 80);

	canny temp10(path[1], 3, 1, 20, 30);
	canny temp11(path[1], 3, 1, 20, 40);
	canny temp12(path[1], 3, 1, 20, 80);
    return 0;
}

