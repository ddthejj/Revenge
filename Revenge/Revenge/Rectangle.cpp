#include "defines.h"
#include "Rectangle.h"

MyRectangle::MyRectangle()
{
	x = 0; y = 0;
	height = 0; width = 0;
}

MyRectangle::MyRectangle(float _x, float _y, float _width, float _height)
{
	x = _x; y = _y;
	height = _height;
	width = _width;
}

MyRectangle::MyRectangle(Point<float> location, float height, float width) : MyRectangle(location.x, location.y, height, width)
{
}

MyRectangle::MyRectangle(const MyRectangle& that)
{
	x = that.X(); y = that.Y();
	height = that.height;
	width = that.width;
}

MyRectangle::~MyRectangle()
{
	//delete location;
}


float MyRectangle::X() const { return x; }
float MyRectangle::Y() const { return y; }
float MyRectangle::Top() const { return y; }
float MyRectangle::Bottom() const { return y + height; }
float MyRectangle::Left() const { return x; }
float MyRectangle::Right() const { return x + width; }
float MyRectangle::CenterX() const { return x + (width / 2.f); }
float MyRectangle::CenterY() const { return y + (height / 2.f); }
Point<float> MyRectangle::Location() const { return Point<float>(x, y); }


void MyRectangle::SetX(float _x) { x = _x; }
void MyRectangle::SetY(float _y) { y = _y; }
void MyRectangle::SetLocation(Point<float> _location) { x = _location.x; y = _location.y; }

void MyRectangle::MoveX(float offset) { x += offset; }
void MyRectangle::MoveY(float offset) { y += offset; }


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