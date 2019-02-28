#include "myCImg.h"

myCImg::myCImg()
{
	image.load_bmp("1.bmp");
}

void myCImg::display()
{
	image.display();
}

//change the black block to red, the white block to blue
void myCImg::changeColor()
{
	cimg_forXY(image, x, y) {
		if (image(x, y, 0) > 140 && image(x, y, 1) > 140 && image(x, y, 2) > 140)
		{
			image(x, y, 0) = 255;
			image(x, y, 1) = image(x, y, 2) = 0;
		}
		else if (image(x, y, 0) < 30 && image(x, y, 1) < 30 && image(x, y, 2) < 30)
		{
			image(x, y, 1) = 255;
			image(x, y, 0) = image(x, y, 2) = 0;
		}
	}
	image.display();
}

void myCImg::drawCircle()
{
	int sum;
	cimg_forXY(image, x, y) {
		// compute the square of distance
		sum = (x - 50)*(x - 50) + (y - 50)*(y - 50);
		if (sum<901)
		{
			image(x, y, 2) = 255;
			image(x, y, 0) = image(x, y, 1) = 0;
		}
	}
	image.display();
}

void myCImg::drawCircle2()
{
	unsigned char blue[] = { 0,0,255 };
	image.draw_circle(50, 50, 30, blue);
	image.display();
}

void myCImg::drawCircleY()
{
	int sum;
	cimg_forXY(image, x, y) {
		// compute the square of distance
		sum = (x - 50)*(x - 50) + (y - 50)*(y - 50);
		if (sum<9.01)
		{
			image(x, y, 2) = 0;
			image(x, y, 0) = image(x, y, 1) = 255;
		}
	}
	image.display();
}

void myCImg::drawCircleY2()
{
	unsigned char yellow[] = { 255,255,0 };
	image.draw_circle(50, 50, 3, yellow);
	image.display();
}

void myCImg::drawLine()
{
	int sum;
	cimg_forXY(image, x, y) {
		sum = x * x + y * y;
		if (sum < 10001 && abs(y - x * tan(pi * 35 / 180))<0.5)
		{
			image(x, y, 2) = 255;
			image(x, y, 0) = image(x, y, 1) = 0;
		}
	}
	image.display();
}

void myCImg::drawLine2()
{
	unsigned char blue[] = { 0,0,255 };
	image.draw_line(0, 0, 100 * cos(pi * 35 / 180), 100 * sin(pi * 35 / 180), blue);
	image.display();
}

void myCImg::save(int i)
{
	// save as different name
	if(i==1)
		image.save("2.bmp");
	else
		image.save("2(1).bmp");
}