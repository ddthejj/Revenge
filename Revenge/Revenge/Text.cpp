#include "defines.h"
#include "Text.h"
#include "Rectangle.h"
#include "SpriteBatch.h"
#include "Manager.h"

Text::Text(std::string _debugName, float x, float y, float width, float height, std::string _text, float _layer, float _opacity, ANCHOR_POINT _anchor) : 
	UISprite(_debugName, x, y, width, height, nullptr, _layer, _opacity, ROTATIONS::NONE, _anchor)
{
	text = _text;
}

Text::Text(std::string _debugName, float x, float y, float width, float height, const char* _text, float _layer, float _opacity, ANCHOR_POINT _anchor) :
	Text(_debugName, x, y, width, height, std::string(_text), _layer, _opacity, _anchor)
{

}

void Text::Draw(SpriteBatch* spriteBatch)
{
	spriteBatch->WriteText(this);
}
