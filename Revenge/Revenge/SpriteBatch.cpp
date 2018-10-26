#pragma once
#include "SpriteBatch.h"
#include "Texture.h"
#include "Rectangle.h"
#include "Renderer.h"


SpriteBatch::SpriteBatch(native_handle hwnd)
{
	renderer = new Renderer((HWND)hwnd);
	camera[0] = WIDTH / 2.0f;
	camera[1] = HEIGHT / 2.0f;
}

SpriteBatch::~SpriteBatch()
{
	delete renderer;
}

Texture * SpriteBatch::Load(const wchar_t * filepath, float height, float width)
{
	return new Texture(renderer->LoadContent(filepath, height, width), height, width);;
}

void SpriteBatch::Begin()
{
	renderer->Begin();
}

void SpriteBatch::Draw(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot)
{
	if (!source)
		renderer->Draw(texture->ID(), ((WIDTH / 2.0f) - camera[0]) + rectangle->X(), ((HEIGHT / 2.0f) - camera[1]) + rectangle->Y(), (int)rectangle->Width(), (int)rectangle->Height(), opacity, layer, rot);
	else
		renderer->Draw(texture->ID(),
			((WIDTH / 2.0f) - camera[0]) + rectangle->X(), ((HEIGHT / 2.0f) - camera[1]) + rectangle->Y(), (int)rectangle->Width(), (int)rectangle->Height(),
			source->X(), source->Y(), source->Height(), source->Width(),
			opacity, layer, rot);
}

void SpriteBatch::End()
{
	renderer->End();
}

void SpriteBatch::MoveCamera(float x, float y)
{
	camera[0] += x; camera[1] += y;
}

void SpriteBatch::SetCamera(float x, float y)
{
	camera[0] = x; camera[1] = y;
}

void SpriteBatch::Resize(native_handle hWnd)
{
	renderer->Resize((HWND)hWnd);
}