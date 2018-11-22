#pragma once
template <typename T>
struct Point;

class MyRectangle
{
protected:
	Point<float>* location;
	float height = 0, width = 0;
public:

	MyRectangle();
	MyRectangle(float _x, float _y, float _height, float _width);
	MyRectangle(Point<float> _location, float height, float width);
	MyRectangle(const MyRectangle& that);
	~MyRectangle();

	float X() const;// { return x; }
	float Y() const;// { return y; }
	float Height() const { return height; }
	float Width() const { return width; }
	float Top() const; //{ return y; }
	float Bottom() const; //{ return y + height; }
	float Left() const;// { return x; }
	float Right() const;// { return x + width; }
	float CenterX() const; //{ return x + (width / 2.f); }
	float CenterY() const; //{ return y + (height / 2.f); }
	Point<float>* Location() const { return location; }

	void SetX(float _x);//{ x = _x; }
	void SetY(float _y); //{ y = _y; }
	void SetWidth(float _width) { width = _width; }
	void SetHeight(float _height) { height = _height; }
	void SetLocation(Point<float> _location);

	void MoveX(float offset);// { x += offset; }
	void MoveY(float offset);// { y += offset; }

	bool Intersects(const MyRectangle &that) const;
};

