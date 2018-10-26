#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Texture.h"

Texture* MenuBox::menuEdge = nullptr;
Texture* MenuBox::menuCorner = nullptr;
Texture* MenuBox::menuBackground = nullptr;


MenuBox::MenuBox(float _x, float _y, float _width, float _height) : Sprite(_x, _y, _width, _height, nullptr, 0.f)
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
	// background
	spriteBatch->Draw(menuBackground, rectangle, 1.0f, .8f);
	// edges
	spriteBatch->Draw(menuEdge, &MyRectangle(rectangle->X(), rectangle->Y(), rectangle->Height(), menuEdge->Width()), 1.f, .81f); // left
	spriteBatch->Draw(menuEdge, &MyRectangle(rectangle->Right() - menuEdge->Width(), rectangle->Y(), rectangle->Height(), menuEdge->Width()), 1.f, .81f, (int)ROTATIONS::HORIZONTAL); // right
	spriteBatch->Draw(menuEdge, &MyRectangle(rectangle->X(), rectangle->Y(), menuEdge->Height(), rectangle->Width()), 1.0f, .81f, (int)ROTATIONS::ROT_90); // top
	spriteBatch->Draw(menuEdge, &MyRectangle(rectangle->X(), rectangle->Bottom() - menuEdge->Height(), menuEdge->Height(), rectangle->Width()), 1.0f, .81f, (int)ROTATIONS::ROT_270); // bottom
	// corners
	spriteBatch->Draw(menuCorner, &MyRectangle(rectangle->X(), rectangle->Y(), menuCorner->Height(), menuCorner->Width()), 1.0f, .82f); // top left
	spriteBatch->Draw(menuCorner, &MyRectangle(rectangle->Right() - menuCorner->Width(), rectangle->Y(), menuCorner->Height(), menuCorner->Width()), 1.0f, .82f, (int)ROTATIONS::ROT_90); // top right
	spriteBatch->Draw(menuCorner, &MyRectangle(rectangle->Right() - menuCorner->Width(), rectangle->Bottom() - menuCorner->Height(), menuCorner->Height(), menuCorner->Width()), 1.0f, .82f, (int)ROTATIONS::ROT_180); // bottom right
	spriteBatch->Draw(menuCorner, &MyRectangle(rectangle->X(), rectangle->Bottom() - menuCorner->Height(), menuCorner->Height(), menuCorner->Width()), 1.0f, .82f, (int)ROTATIONS::ROT_270); // bottom left
}