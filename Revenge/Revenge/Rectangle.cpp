#include "Rectangle.h"



MyRectangle::MyRectangle()
{
	x = 0; y = 0; height = 0; width = 0;
}

MyRectangle::MyRectangle(float _x, float _y, float _height, float _width)
{
	x = _x;
	y = _y;
	height = _height;
	width = _width;
}


MyRectangle::~MyRectangle()
{
}

bool MyRectangle::Intersects(const MyRectangle &that) const
{
	if (Top() <= that.Bottom() && Top() >= that.Top())
	{
		if (Left() <= that.Right() && Left() >= that.Left())
		{
			return true;
		}
		else if (Right() <= that.Right() && Right() >= that.Left())
		{
			return true;
		}
	}
	else if (Bottom() <= that.Bottom() && Bottom() >= that.Top())
	{
		if (Left() <= that.Right() && Left() >= that.Left())
		{
			return true;
		}
		else if (Right() <= that.Right() && Right() >= that.Left())
		{
			return true;
		}
	}
	return false;
}