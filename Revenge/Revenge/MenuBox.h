#pragma once
#include "Sprite.h"

class MenuBox : public Sprite
{
public:

	MenuBox(float _x, float _y, float _width, float _height, Texture* _texture);
	~MenuBox();

	virtual void Draw(SpriteBatch* spriteBatch);
	virtual void Update();
};

