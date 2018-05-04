#pragma once

struct D2D_RECT_F;

class MyRectangle
{
	float x, y, width, height;

public:
	MyRectangle(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f) { this->x = x; this->y = y; this->width = width; this->height = height; }
	~MyRectangle();

	float X() { return x; }
	float Y() { return y; }
	float Height() { return height; }
	float Width() { return width; }
	float Top() { return y; }
	float Bottom() { return y + height; }
	float Left() { return x; }
	float Right() { return x + width; }

	D2D_RECT_F d2d1RECT();
};

