#include "defines.h"
#include "Sprite.h"
#include "Manager.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Rectangle.h"
#include <algorithm>

Sprite::Sprite(std::string _debugName, float x, float y, float width, float height, const Texture* _texture, float _layer, float _opacity, ROTATIONS _rotation) : Sprite(_debugName, x, y, width, height, 0, 0,
	_texture ? (_texture->SourceRectangle() ? _texture->SourceRectangle()->Width() : _texture->Width()) : 0, 
	_texture ? (_texture->SourceRectangle() ? _texture->SourceRectangle()->Height() : _texture->Height()) : 0, 
	_texture, _layer, _opacity, _rotation)
{

}

Sprite::Sprite(std::string _debugName, float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, const Texture* _texture, float _layer, float _opacity, ROTATIONS _rotation) : Object(_debugName)
{
	rectangle = new MyRectangle(x, y, width, height);
	sourceRectangle = new MyRectangle(sX, sY, sWidth, sHeight);
	texture = _texture;
	layer = _layer;
	opacity = _opacity;
	rotation = _rotation;
}

Sprite::Sprite(const Sprite& _that) : Object(_that.debugName)
{
	rectangle = new MyRectangle(*_that.GetRectangle());
	sourceRectangle = new MyRectangle(*_that.GetSourceRectangle());
	texture = _that.GetTexture();
	layer = _that.GetLayer();
	opacity = _that.GetOpacity();
	rotation = _that.GetRotation();
}

Sprite::~Sprite()
{
	if (active) Deactivate();

	delete rectangle;
	delete sourceRectangle;
}

Point<float> Sprite::GetPos() const
{
	return rectangle->Location();
}

Point<float> Sprite::GetSourcePos() const
{
	return sourceRectangle->Location();
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

void Sprite::SetOpacity(float _opacity)
{
	opacity = std::clamp(_opacity, 0.f, 1.f);
}

void Sprite::DecreaseOpacity(float amnt)
{
	opacity = std::clamp(opacity - amnt, 0.f, 1.f);
}

void Sprite::IncreaseOpacity(float amnt)
{
	opacity = std::clamp(opacity + amnt, 0.f, 1.f);
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
	spriteBatch->Draw(this);
}


UISprite::UISprite(std::string _debugName, float x, float y, float width, float height, const Texture* _texture, float _layer, float _opacity, ROTATIONS _rotation, ANCHOR_POINT _anchor) :
	Sprite(_debugName, x, y, width, height, _texture, _layer, _opacity, _rotation)
{
	anchor = _anchor;
}

UISprite::UISprite(std::string _debugName, float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, const Texture* _texture, float _layer, float _opacity, ROTATIONS _rotation, ANCHOR_POINT _anchor) :
	Sprite(_debugName, x, y, width, height, sX, sY, sWidth, sHeight, _texture, _layer, _opacity, _rotation)
{
	anchor = _anchor;
}

UISprite::UISprite(const UISprite& _that) : Sprite(_that)
{
	anchor = _that.GetAnchorPoint();
}

void UISprite::Deactivate()
{
	Sprite::Deactivate();
}

void UISprite::Draw(SpriteBatch* spriteBatch)
{
	spriteBatch->DrawUI(this);
}

void UISprite::DrawInSprite(SpriteBatch* spriteBatch, UISprite* parent)
{
	spriteBatch->DrawUIInSprite(this, parent);
}
