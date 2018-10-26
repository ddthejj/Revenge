#pragma once
#include "Sprite.h"

class MenuBox : public Sprite
{
	static Texture* menuEdge;
	static Texture* menuCorner;
	static Texture* menuBackground;

public:

	static void SetMenuTextures(Texture* edge, Texture* corner, Texture* background) { menuEdge = edge; menuCorner = corner; menuBackground = background; }

	MenuBox(float _x, float _y, float _width, float _height);
	~MenuBox();

	virtual void Draw(SpriteBatch* spriteBatch);
	virtual void Update();
};

