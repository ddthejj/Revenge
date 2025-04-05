#pragma once
#include "Sprite.h"
#include <string>

/*
class MyRectangle;
class SpriteBatch;
template<typename T>
struct Point;
*/

class Text : public UISprite
{
protected:
	std::string text = "";

public:
	Text(std::string _debugName, float x, float y, float width, float height, std::string _text = "", float _layer = 1.f, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	Text(std::string _debugName, float x, float y, float width, float height, const char* _text, float _layer = 1.f, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);

	void SetText(std::string _text) { text = _text; }
	void SetText(const char* _text) { text = std::string(_text); }
	std::string GetText() const { return text; }

	virtual void Draw(SpriteBatch* spriteBatch);
};

