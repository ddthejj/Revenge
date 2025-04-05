#include "BorderedBox.h"
#include "defines.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Rectangle.h"

BorderedBox::BorderedBox(std::string _debugName, float x, float y, float width, float height, const Texture* _texture, float _layer, float _opacity, ANCHOR_POINT _anchor)
	: UISprite(_debugName, x, y, width, height, _texture, _layer, _opacity, ROTATIONS::NONE, _anchor)
{
}

BorderedBox::BorderedBox(std::string _debugName, float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, const Texture* _texture, float _layer, float _opacity, ANCHOR_POINT _anchor)
	: UISprite(_debugName, x, y, width, height, sX, sY, sWidth, sHeight, _texture, _layer, _opacity, ROTATIONS::NONE, _anchor)
{
}

void BorderedBox::DrawBox(SpriteBatch* spriteBatch)
{
	if (texture)
	{
		// source rectangles for menu parts
		MyRectangle cornerSource(0, 0, 10, 10);
		MyRectangle borderSource(10, 0, 10, 10);
		MyRectangle backgroundSource(20, 0, 10, 10);

		MyRectangle screenspaceRec = GetScreenLocation(*rectangle, anchor);

		// background

		UISprite tempSprite = UISprite(
			debugName + "DrawSprite",
			rectangle->X(), rectangle->Y(), rectangle->Width(), rectangle->Height(),
			backgroundSource.X(), backgroundSource.Y(), backgroundSource.Width(), backgroundSource.Height(),
			texture, layer, opacity, ROTATIONS::NONE, anchor);
		spriteBatch->DrawUI(&tempSprite);
		
		// edges

		tempSprite.SetRectangle(MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), borderSource.Width(), rectangle->Height()));
		tempSprite.SetSource(borderSource);
		tempSprite.SetLayer(layer + .2f);
		tempSprite.SetAnchorPoint(ANCHOR_POINT::ANCHOR_TOP_LEFT);
		spriteBatch->DrawUI(&tempSprite);

		tempSprite.GetSourceRectangle()->SetY(10);
		tempSprite.SetRectangle(MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), rectangle->Width(), borderSource.Height()));
		tempSprite.SetRotation(ROTATIONS::ROT_90);
		spriteBatch->DrawUI(&tempSprite);

		tempSprite.GetSourceRectangle()->SetY(20);
		tempSprite.SetRectangle(MyRectangle(screenspaceRec.Right() - borderSource.Width(), screenspaceRec.Y(), borderSource.Width(), rectangle->Height()));
		tempSprite.SetRotation(ROTATIONS::HORIZONTAL);
		spriteBatch->DrawUI(&tempSprite);

		tempSprite.GetSourceRectangle()->SetY(30);
		tempSprite.SetRectangle(MyRectangle(screenspaceRec.X(), screenspaceRec.Bottom() - borderSource.Height(), rectangle->Width(), borderSource.Height()));
		tempSprite.SetRotation(ROTATIONS::ROT_270);
		spriteBatch->DrawUI(&tempSprite);
		
		// corners

		tempSprite.SetRectangle(MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), cornerSource.Width(), cornerSource.Height()));
		tempSprite.SetSource(cornerSource);
		tempSprite.SetLayer(layer + .3f);
		tempSprite.SetRotation(ROTATIONS::NONE);
		spriteBatch->DrawUI(&tempSprite);

		tempSprite.GetSourceRectangle()->SetY(10);
		tempSprite.SetRectangle(MyRectangle(screenspaceRec.Right() - cornerSource.Width(), screenspaceRec.Y(), cornerSource.Width(), cornerSource.Height()));
		tempSprite.SetRotation(ROTATIONS::ROT_90);
		spriteBatch->DrawUI(&tempSprite);

		tempSprite.GetSourceRectangle()->SetY(20);
		tempSprite.SetRectangle(MyRectangle(screenspaceRec.Right() - cornerSource.Width(), screenspaceRec.Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height()));
		tempSprite.SetRotation(ROTATIONS::ROT_180);
		spriteBatch->DrawUI(&tempSprite);

		tempSprite.GetSourceRectangle()->SetY(30);
		tempSprite.SetRectangle(MyRectangle(screenspaceRec.X(), screenspaceRec.Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height()));
		tempSprite.SetRotation(ROTATIONS::ROT_270);
		spriteBatch->DrawUI(&tempSprite);
	}
}
