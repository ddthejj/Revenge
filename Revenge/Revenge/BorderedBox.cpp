#include "BorderedBox.h"
#include "defines.h"
#include "SpriteBatch.h"
#include "Texture.h"

BorderedBox::BorderedBox(float x, float y, float width, float height, Texture* _texture, float _layer, float _opacity, ANCHOR_POINT _anchor) 
	: UISprite(x, y, width, height, _texture, _layer, _opacity, _anchor)
{
}

BorderedBox::BorderedBox(float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, Texture* _texture, float _layer, float _opacity, ANCHOR_POINT _anchor) 
	: UISprite(x, y, width, height, sX, sY, sWidth, sHeight, _texture, _layer, _opacity, _anchor)
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
		spriteBatch->DrawUI(texture, rectangle, &backgroundSource, opacity, layer, 0, anchor);
		// edges
		MyRectangle edgeRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), borderSource.Width(), rectangle->Height());
		spriteBatch->DrawUI(texture, &edgeRectangle, &borderSource, opacity, layer + .2f, 0); // left
		borderSource.SetY(10);
		edgeRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), rectangle->Width(), borderSource.Height());
		spriteBatch->DrawUI(texture, &edgeRectangle, &borderSource, opacity, layer + .2f, (int)ROTATIONS::ROT_90); // top
		borderSource.SetY(20);
		edgeRectangle = MyRectangle(screenspaceRec.Right() - borderSource.Width(), screenspaceRec.Y(), borderSource.Width(), rectangle->Height());
		spriteBatch->DrawUI(texture, &edgeRectangle, &borderSource, opacity, layer + .2f, (int)ROTATIONS::HORIZONTAL); // right
		borderSource.SetY(30);
		edgeRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Bottom() - borderSource.Height(), rectangle->Width(), borderSource.Height());
		spriteBatch->DrawUI(texture, &edgeRectangle, &borderSource, opacity, layer + .2f, (int)ROTATIONS::ROT_270); // bottom
		// corners
		MyRectangle cornerRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), cornerSource.Width(), cornerSource.Height());
		spriteBatch->DrawUI(texture, &cornerRectangle, &cornerSource, opacity, layer + .3f, 0); // top left
		cornerSource.SetY(10);
		cornerRectangle = MyRectangle(screenspaceRec.Right() - cornerSource.Width(), screenspaceRec.Y(), cornerSource.Width(), cornerSource.Height());
		spriteBatch->DrawUI(texture, &cornerRectangle, &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_90); // top right
		cornerSource.SetY(20);
		cornerRectangle = MyRectangle(screenspaceRec.Right() - cornerSource.Width(), screenspaceRec.Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height());
		spriteBatch->DrawUI(texture, &cornerRectangle, &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_180); // bottom right
		cornerSource.SetY(30);
		cornerRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height());
		spriteBatch->DrawUI(texture, &cornerRectangle, &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_270); // bottom left
	}
}
