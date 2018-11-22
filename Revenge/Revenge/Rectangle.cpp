#include "defines.h"
#include "Rectangle.h"

MyRectangle::MyRectangle()
{
	location = new Point();
	height = 0; width = 0;
}

MyRectangle::MyRectangle(float _x, float _y, float _height, float _width)
{
	location->x = _x;
	location->y = _y;
	height = _height;
	width = _width;
}

MyRectangle::MyRectangle(Point location, float height, float width)
{
	MyRectangle(location.x, location.y, height, width);
}

MyRectangle::~MyRectangle()
{
	delete location;
}


float MyRectangle::X() const { return location->x; }
float MyRectangle::Y() const { return location->y; }
float MyRectangle::Top() const { return location->y; }
float MyRectangle::Bottom() const { return location->y + height; }
float MyRectangle::Left() const { return location->x; }
float MyRectangle::Right() const { return location->x + width; }
float MyRectangle::CenterX() const { return location->x + (width / 2.f); }
float MyRectangle::CenterY() const { return location->y + (height / 2.f); }

void MyRectangle::SetX(float _x) { location->x = _x; }
void MyRectangle::SetY(float _y) { location->y = _y; }

void MyRectangle::MoveX(float offset) { location->x += offset; }
void MyRectangle::MoveY(float offset) { location->y += offset; }


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