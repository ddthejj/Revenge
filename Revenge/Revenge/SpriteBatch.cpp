#pragma once
#include "defines.h"
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

Texture* SpriteBatch::Load(const wchar_t* filepath, float height, float width)
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
	{
		if (texture->SourceRectangle())
		{
			renderer->Draw(
				texture->ID(),
				((windowWidth / 2.0f) - camera[0]) + rectangle->X(),
				((windowHeight / 2.0f) - camera[1]) + rectangle->Y(),
				(int)rectangle->Width(),
				(int)rectangle->Height(),
				texture->SourceRectangle()->X(), 
				texture->SourceRectangle()->Y(),
				(int)texture->SourceRectangle()->Width(), 
				(int)texture->SourceRectangle()->Height(),
				opacity, layer, rot);
		}
		else
		{
			renderer->Draw(
				texture->ID(),
				((windowWidth / 2.0f) - camera[0]) + rectangle->X(),
				((windowHeight / 2.0f) - camera[1]) + rectangle->Y(),
				(int)rectangle->Width(),
				(int)rectangle->Height(),
				0.f, 0.f, (int)texture->Width(), (int)texture->Height(),
				opacity, layer, rot);
		}
	}
	else
	{
		if (texture->SourceRectangle())
		{
			renderer->Draw(texture->ID(),
				((windowWidth / 2.0f) - camera[0]) + rectangle->X(),
				((windowHeight / 2.0f) - camera[1]) + rectangle->Y(),
				(int)rectangle->Width(), (int)rectangle->Height(),
				source->X() + texture->SourceRectangle()->X(), 
				source->Y() + texture->SourceRectangle()->Y(), 
				(int)source->Height(), 
				(int)source->Width(),
				opacity, layer, rot);
		}
		else
		{
			renderer->Draw(texture->ID(),
				((windowWidth / 2.0f) - camera[0]) + rectangle->X(),
				((windowHeight / 2.0f) - camera[1]) + rectangle->Y(),
				(int)rectangle->Width(), (int)rectangle->Height(),
				source->X(), source->Y(), (int)source->Height(), (int)source->Width(),
				opacity, layer, rot);
		}
	}
}

void SpriteBatch::DrawUI(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot)
{
	if (!source)
		renderer->Draw(texture->ID(),
			rectangle->X(), rectangle->Y(), (int)rectangle->Width(), (int)rectangle->Height(),
			0.f, 0.f, (int)texture->Width(), (int)texture->Height(),
			opacity, layer, rot);
	else
		renderer->Draw(texture->ID(),
			rectangle->X(), rectangle->Y(), (int)rectangle->Width(), (int)rectangle->Height(),
			source->X(), source->Y(), (int)source->Width(), (int)source->Height(),
			opacity, layer, rot);
}

void SpriteBatch::WriteText(const char* text, MyRectangle* rectangle, float layer)
{
	renderer->Write(text, rectangle->X(), rectangle->Y(), rectangle->Height(), rectangle->Width(), layer);
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
	RECT rc;
	GetClientRect((HWND)hWnd, &rc);

	windowWidth = rc.right - rc.left;
	windowHeight = rc.bottom - rc.top;

	SetCamera((float)windowWidth / 2.f, (float)windowHeight / 2.f);
}



Point<float> SpriteBatch::MeasureString(std::string text)
{
	return renderer->MeasureString(text, (float)windowWidth, (float)windowHeight);
}
