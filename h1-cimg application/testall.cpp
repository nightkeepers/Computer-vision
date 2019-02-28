#include "myCImg.h"



int main()
{
	myCImg temp, temp2;
	temp.display();
	temp2.display();
	temp.changeColor();
	temp2.changeColor();
	temp.drawCircle();
	temp2.drawCircle2();
	temp.drawCircleY();
	temp2.drawCircleY2();
	temp.drawLine();
	temp2.drawLine2();
	temp.save(1);
	temp.save(2);
	return 0;
}