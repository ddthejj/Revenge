#pragma once
#include "Object.h"

class MyRectangle;
class Texture;
template<typename T>
struct Point;

class Sprite : public Object
{
protected:
	MyRectangle* rectangle, *sourceRectangle;
	Texture* texture;
	float layer = 0.f;
public:
	float opacity = 1.f;
	Sprite(float x, float y, float width, float height, Texture* _texture, float _layer, float _opacity = 1.f);
	Sprite(float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, Texture* _texture, float _layer, float _opacity = 1.f);
	~Sprite();

	MyRectangle* GetRectangle() const;
	void SetRectangle(const MyRectangle& _rectangle);
	void SetPos(const Point<float>& location);

	virtual void Draw(SpriteBatch* spriteBatch);
	virtual void Update() {}
	virtual void Activate();
	virtual void Deactivate();
	virtual void Freeze();
	virtual void Unfreeze();
};

