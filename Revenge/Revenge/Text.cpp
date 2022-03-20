#include "defines.h"
#include "Text.h"
#include "Rectangle.h"
#include "SpriteBatch.h"
#include "Manager.h"

Text::Text(float x, float y, float width, float height, std::string _text, float _layer, float _opacity, ANCHOR_POINT _anchor)
{
	rectangle = new MyRectangle(x, y, width, height);
	text = _text;
	layer = _layer;
	opacity = _opacity;
	anchor = _anchor;
}

Text::Text(float x, float y, float width, float height, const char* _text, float _layer, float _opacity, ANCHOR_POINT _anchor) :
	Text(x, y, width, height, std::string(_text), _layer, _opacity, _anchor)
{

}

Text::~Text()
{
	if (active)
		Deactivate();

	SafeDelete(rectangle);
}

void Text::SetRectangle(MyRectangle _rectangle)
{
	*rectangle = _rectangle;
}

void Text::SetLocation(Point<float> location)
{
	rectangle->SetLocation(location);
}

Point<float> Text::Location() const
{
	return rectangle->Location();
}

void Text::SetX(float x)
{
	rectangle->SetX(x);
}

float Text::X() const
{
	return rectangle->X();
}

void Text::SetY(float y)
{
	rectangle->SetY(y);
}

float Text::Y() const
{
	return rectangle->Y();
}

void Text::Activate()
{
	if (!active)
	{
		Manager::AddUpdate(this);
		Manager::AddDraw(this);
		Object::Activate();
	}
}

void Text::Deactivate()
{
	if (active)
	{
		if (!frozen)
			Manager::RemoveUpdate(this);
		Manager::RemoveDraw(this);
		Object::Deactivate();
	}
}

void Text::Freeze()
{
	if (!frozen)
	{
		Manager::RemoveUpdate(this);
		Object::Freeze();
	}
}

void Text::Unfreeze()
{
	if (frozen)
	{
		Manager::AddUpdate(this);
		Object::Unfreeze();
	}
}

void Text::Draw(SpriteBatch* spriteBatch)
{
	spriteBatch->WriteText(this);
}
