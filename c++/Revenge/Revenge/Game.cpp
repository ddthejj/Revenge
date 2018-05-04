#include "Game.h"
#include "Texture.h"
#include "MyRectangle.h"
#include "SpriteBatch.h"

#include <Windows.h>
#include <d2d1.h>

Texture* sprite;
MyRectangle rectangle;
MyRectangle rectangle2;

Game::Game(SpriteBatch* gfx)
{
	spriteBatch = gfx;

	sprite = new Texture(L"test.bmp");
	rectangle = MyRectangle(0.0f, 0.0f, sprite->Bitmap()->GetSize().width, sprite->Bitmap()->GetSize().height);
	rectangle2 = MyRectangle(100.0f, 100.0f, sprite->Bitmap()->GetSize().width, sprite->Bitmap()->GetSize().height);
}


Game::~Game()
{
}

void Game::Update()
{

}

void Game::Draw()
{
	spriteBatch->Begin();
	spriteBatch->ClearScreen(0.0f, 0.0f, 0.0f);

	spriteBatch->Draw(sprite, rectangle, rectangle, 1);
	spriteBatch->Draw(sprite, rectangle2, rectangle2, 1);
	
	spriteBatch->End();
}