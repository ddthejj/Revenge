#pragma once

#include "Sprite.h"


class BorderedBox : public UISprite
{
public:
	BorderedBox(std::string _debugName, float x, float y, float width, float height, const Texture* _texture, float _layer, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	BorderedBox(std::string _debugName, float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, const Texture* _texture, float _layer, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	~BorderedBox() = default;

protected: 
	void DrawBox(SpriteBatch* spriteBatch);
};