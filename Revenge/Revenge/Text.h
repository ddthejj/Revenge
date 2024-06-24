#pragma once
#include "Object.h"
#include <string>

class MyRectangle;
class SpriteBatch;
template<typename T>
struct Point;

class Text : public Object
{
protected:
	MyRectangle* rectangle = nullptr;
	std::string text = "";
	float layer = 0.f;
	float opacity = 1.f;
	ANCHOR_POINT anchor;

public:
	Text(std::string _debugName, float x, float y, float width, float height, std::string _text = "", float _layer = 1.f, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	Text(std::string _debugName, float x, float y, float width, float height, const char* _text, float _layer = 1.f, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	~Text();
	// update the text
	virtual void Update(float delta_time) {}
	// activate the text
	virtual void Activate();
	// deactivate the text
	virtual void Deactivate();
	// freeze the sprite
	virtual void Freeze();
	// unfreeze the sprite
	virtual void Unfreeze();

	void SetRectangle(MyRectangle _rectangle);
	const MyRectangle* Rectangle() const { return rectangle; }
	void SetLocation(Point<float> location);
	Point<float> Location() const;
	void SetX(float x);
	float X() const;
	void SetY(float y);
	float Y() const;
	void SetText(std::string _text) { text = _text; }
	void SetText(const char* _text) { text = std::string(_text); }
	std::string GetText() const { return text; }
	void SetLayer(float _layer) { layer = _layer; }
	float Layer() const { return layer; }
	void SetOpacity(float _opacity) { opacity = _opacity; }
	float Opacity() const { return opacity; }
	void SetAnchor(ANCHOR_POINT _anchor) { anchor = _anchor; }
	ANCHOR_POINT Anchor() const { return anchor; }

	virtual void Draw(SpriteBatch* spriteBatch);
};

