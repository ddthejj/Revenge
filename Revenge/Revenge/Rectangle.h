#pragma once
class MyRectangle
{
protected:
	float x = 0, y = 0, height = 0, width = 0;
public:

	MyRectangle();
	MyRectangle(float _x, float _y, float _height, float _width);
	~MyRectangle();

	float X() const { return x; }
	float Y() const { return y; }
	float Height() const { return height; }
	float Width() const { return width; }
	float Top() const { return y; }
	float Bottom() const { return y + height; }
	float Left() const { return x; }
	float Right() const { return x + width; }
	float CenterX() const { return x + (width / 2.f); }
	float CenterY() const { return y + (height / 2.f); }

	void SetX(float _x) { x = _x; }
	void SetY(float _y) { y = _y; }
	void SetWidth(float _width) { width = _width; }
	void SetHeight(float _height) { height = _height; }

	void MoveX(float offset) { x += offset; }
	void MoveY(float offset) { y += offset; }

	bool Intersects(const MyRectangle &that) const;
};

