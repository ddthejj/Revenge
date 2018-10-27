#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Texture.h"

MenuBox::MenuBox(float _x, float _y, float _width, float _height, Texture* _texture) : Sprite(_x, _y, _width, _height, _texture, 0.f)
{

}


MenuBox::~MenuBox()
{
}

void MenuBox::Update()
{

}

void MenuBox::Draw(SpriteBatch* spriteBatch)
{

	MyRectangle cornerSource(0, 0, 10, 10);
	MyRectangle borderSource(10, 0, 10, 10);
	MyRectangle backgroundSource(20, 0, 10, 10);

	// background
	spriteBatch->DrawUI(texture, rectangle, &backgroundSource, 1.0f, .8f, 0);
	// edges
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), rectangle->Height(), borderSource.Width()), &borderSource, 1.f, .82f); // left
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - borderSource.Width(), rectangle->Y(), rectangle->Height(), borderSource.Width()), &borderSource, 1.f, .82f, (int)ROTATIONS::HORIZONTAL); // right
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), borderSource.Height(), rectangle->Width()), &borderSource, 1.0f, .82f, (int)ROTATIONS::ROT_90); // top
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Bottom() - borderSource.Height(), borderSource.Height(), rectangle->Width()), &borderSource, 1.0f, .82f, (int)ROTATIONS::ROT_270); // bottom
	// corners
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), cornerSource.Height(), cornerSource.Width()), &cornerSource, 1.0f, .83f); // top left
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - cornerSource.Width(), rectangle->Y(), cornerSource.Height(), cornerSource.Width()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_90); // top right
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - cornerSource.Width(), rectangle->Bottom() - cornerSource.Height(), cornerSource.Height(), cornerSource.Width()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_180); // bottom right
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Bottom() - cornerSource.Height(), cornerSource.Height(), cornerSource.Width()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_270); // bottom left
	// text
	spriteBatch->WriteText(L"Menu", &MyRectangle(rectangle->X() + 20, rectangle->Y() + 20, 30, 10), .81f);


}