#pragma once
#include "Sprite.h"

class MenuBox : public Sprite
{
	friend class MenuManager;

protected:

	MenuBox* previousMenu;

public:

	MenuBox(float _x, float _y, float _width, float _height, Texture* _texture);
	~MenuBox();

	void Open(MenuBox* _previousMenu);

	virtual void Draw(SpriteBatch* spriteBatch);
	virtual void Update();
};

