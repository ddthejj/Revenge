#include "defines.h"
#include "DialogBox.h"
#include "Texture.h"
#include "SpriteBatch.h"

DialogBox::DialogBox(float _x, float _y, float _width, float _height, Texture* _texture, Texture* _arrowTexture, float _layer, float _opacity, ANCHOR_POINT _anchor) : UISprite(_x, _y, _width, _height, _texture, _layer, _opacity, _anchor)
{
	arrow = new UISprite(0, 0, 7, 7, 0, 0, 15, 15, _arrowTexture, _layer + .01f, _opacity, _anchor);
}

DialogBox::~DialogBox()
{
	if (active) Deactivate();
}

void DialogBox::Open()
{
}

void DialogBox::Draw(SpriteBatch* spriteBatch)
{
	if (texture)
	{
		// source rectangles for box parts
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

	// write text

	// draw the arrow
	arrow->Draw(spriteBatch);
}

void DialogBox::Update()
{
}

void DialogBox::SetText(Character* _speaker, std::vector<std::vector<std::string>> _text)
{
	speaker = _speaker;
	text = _text;
}

void DialogBox::SelectPressed()
{
}
