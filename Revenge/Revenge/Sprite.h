#pragma once
#include "Object.h"

class MyRectangle;
class Texture;

class Sprite : public Object
{
protected:
	MyRectangle* rectangle;
	Texture* texture;
	float layer = 0.f;
public:
	float opacity = 1.f;
	Sprite(float x, float y, float width, float height, Texture* _texture, float _layer, float _opacity = 1.f);
	~Sprite();

	MyRectangle* GetRectangle();

	virtual void Draw(SpriteBatch* spriteBatch);
	virtual void Update() {}
	virtual void Activate();
	virtual void Deactivate();
	virtual void Freeze();
	virtual void Unfreeze();
};

