#pragma once
template <typename T>
struct Point;

// basic rectangle class
class MyRectangle
{
protected:

#pragma region Properties

	// the location of the rectangle
	float x, y;
	// the dimensions of the rectangle
	float height = 0, width = 0;

#pragma endregion

public:

#pragma region Methods

	MyRectangle();
	MyRectangle(float _x, float _y, float _width, float _height);
	MyRectangle(Point<float> _location, float height, float width);
	MyRectangle(const MyRectangle& that);
	~MyRectangle();

	float X() const;										// returns the rectangle's x position
	float Y() const;										// returns the rectangle's y position
	float Height() const { return height; }					// returns the rectangle's height
	float Width() const { return width; }					// returns the rectangle's width
	float Top() const;										// returns the top x of the rectangle
	float Bottom() const;									// returns the bottom x of the rectangle
	float Left() const;										// returns the left y of the rectangle
	float Right() const;									// returns the right y of the rectangle
	float CenterX() const;									// returns the center x of the rectangle
	float CenterY() const;									// returns the center y of the rectangle
	Point<float> Location() const;							// returns the location of the rectangle

	void SetX(float _x);
	void SetY(float _y);
	void SetWidth(float _width) { width = _width; }
	void SetHeight(float _height) { height = _height; }
	void SetLocation(Point<float> _location);

	void MoveX(float offset);// { x += offset; }
	void MoveY(float offset);// { y += offset; }

	bool Intersects(const MyRectangle &that) const;

#pragma endregion

};

