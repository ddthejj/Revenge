#include "defines.h"
#include "Sprite.h"
#include "Manager.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Rectangle.h"

Sprite::Sprite(float x, float y, float width, float height, Texture* _texture, float _layer, float _opacity) : Sprite(x, y, width, height, 0, 0,
	// if the texture has a default source rectangle, use it as the sprite's source rectangle, but if it doesn't, just use the normal texture height / width 
	_texture ? (_texture->SourceRectangle() ? _texture->SourceRectangle()->Width() : _texture->Width()) : 0, 
	_texture ? (_texture->SourceRectangle() ? _texture->SourceRectangle()->Height() : _texture->Height()) : 0, 
	_texture, _layer, _opacity)
{
	// just call the constructor of Object and don't do anything special in the sprite class itself
}

Sprite::Sprite(float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, Texture* _texture, float _layer, float _opacity)
{
	rectangle = new MyRectangle(x, y, width, height);
	sourceRectangle = new MyRectangle(sX, sY, sWidth, sHeight);
	texture = _texture;
	layer = _layer;
	opacity = _opacity;
}

Sprite::~Sprite()
{
	if (active) Deactivate();

	delete rectangle;
	delete sourceRectangle;
}

MyRectangle* Sprite::GetRectangle() const
{
	return rectangle;
}

Point<float> Sprite::GetPos() const
{
	return rectangle->Location();
}

void Sprite::SetRectangle(const MyRectangle& _rectangle)
{
	if (rectangle)
		delete rectangle;
	rectangle = new MyRectangle(_rectangle);
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
	{
		spriteBatch->Draw(texture, rectangle, opacity, layer);
	}
	else
	{
		spriteBatch->Draw(texture, rectangle, sourceRectangle, opacity, layer);
	}
}


UISprite::UISprite(float x, float y, float width, float height, Texture* _texture, float _layer, float _opacity, ANCHOR_POINT _anchor) :
	Sprite(x, y, width, height, _texture, _layer, _opacity)
{
	anchor = _anchor;
}

UISprite::UISprite(float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, Texture* _texture, float _layer, float _opacity, ANCHOR_POINT _anchor) :
	Sprite(x, y, width, height, sX, sY, sWidth, sHeight, _texture, _layer, _opacity)
{
	anchor = _anchor;
}

void UISprite::Deactivate()
{
	Sprite::Deactivate();
}

void UISprite::Draw(SpriteBatch* spriteBatch)
{
	if (!sourceRectangle)
		spriteBatch->DrawUI(texture, rectangle, opacity, layer, 0, anchor);
	else
		spriteBatch->DrawUI(texture, rectangle, sourceRectangle, opacity, layer, 0, anchor);
}

void UISprite::DrawInSprite(SpriteBatch* spriteBatch, UISprite* parent)
{
	if (!sourceRectangle)
		spriteBatch->DrawUIInSprite(texture, rectangle, parent, opacity, layer, 0, anchor);
	else
		spriteBatch->DrawUIInSprite(texture, rectangle, sourceRectangle, parent, opacity, layer, 0, anchor);
}
