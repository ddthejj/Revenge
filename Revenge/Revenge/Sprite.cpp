#include "defines.h"
#include "Sprite.h"
#include "Manager.h"
#include "SpriteBatch.h"
#include "Texture.h"

Sprite::Sprite(float x, float y, float width, float height, Texture* _texture, float _layer, float _opacity) : Sprite(x, y, width, height, 0, 0, _texture->Width(), _texture->Height(), _texture, _layer, _opacity)
{

}

Sprite::Sprite(float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, Texture * _texture, float _layer, float _opacity)
{
	rectangle = new MyRectangle(x, y, width, height);
	sourceRectangle = new MyRectangle(sX, sY, sHeight, sWidth);
	texture = _texture;
	layer = _layer;
	opacity = _opacity;
}

Sprite::~Sprite()
{
	delete rectangle;
	delete sourceRectangle;
}

MyRectangle* Sprite::GetRectangle() const
{
	return rectangle;
}

void Sprite::SetRectangle(const MyRectangle& _rectangle)
{
	*rectangle = _rectangle;
}

void Sprite::SetPos(const Point<float>& location)
{
	rectangle->SetLocation(location);
}

void Sprite::SetSource(const MyRectangle& _source)
{
	*sourceRectangle = _source;
}

void Sprite::SetSourcePos(const Point<float>& location)
{
	sourceRectangle->SetLocation(location);
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
	if (!sourceRectangle)
		spriteBatch->Draw(texture, rectangle, opacity, layer);
	else
		spriteBatch->Draw(texture, rectangle, sourceRectangle, opacity, layer);
}

void Sprite::DrawUI(SpriteBatch* spriteBatch)
{
	if (!sourceRectangle)
		spriteBatch->DrawUI(texture, rectangle, opacity, layer);
	else
		spriteBatch->DrawUI(texture, rectangle, sourceRectangle, opacity, layer);
}