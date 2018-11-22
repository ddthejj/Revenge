#include "defines.h"
#include "Sprite.h"
#include "Manager.h"
#include "SpriteBatch.h"

Sprite::Sprite(float x, float y, float width, float height, Texture* _texture, float _layer, float _opacity)
{
	rectangle = new MyRectangle(x, y, width, height);
	texture = _texture;
	layer = _layer;
	opacity = _opacity;
}

Sprite::~Sprite()
{
	delete rectangle;
}

MyRectangle* Sprite::GetRectangle()
{
	return rectangle;
}

void Sprite::Activate()
{
	if (!active)
	{
		Manager::AddUpdate(this);
		Manager::AddDraw(this);
		Object::Activate();
	}
}

void Sprite::Deactivate()
{
	if (active)
	{
		if (!frozen)
			Manager::RemoveUpdate(this);
		Manager::RemoveDraw(this);
		Object::Deactivate();
	}
}

void Sprite::Freeze()
{
	if (!frozen)
	{
		Manager::RemoveUpdate(this);
		Object::Freeze();
	}
}

void Sprite::Unfreeze()
{
	if (frozen)
	{
		Manager::AddUpdate(this);
		Object::Unfreeze();
	}
}

void Sprite::Draw(SpriteBatch* spriteBatch)
{
	spriteBatch->Draw(texture, rectangle, opacity, layer);
}