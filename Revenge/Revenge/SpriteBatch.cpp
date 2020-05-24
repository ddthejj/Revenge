#pragma once
#include "defines.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "Text.h"


SpriteBatch::SpriteBatch(native_handle hwnd)
{
	renderer = new Renderer((HWND)hwnd);
	//camera[0] = WIDTH / 2.0f;
	//camera[1] = HEIGHT / 2.0f;

	RECT rc;
	GetClientRect((HWND)hwnd, &rc);

	windowWidth = rc.right - rc.left;
	windowHeight = rc.bottom - rc.top;

	SetCamera((float)windowWidth / 2.f, (float)windowHeight / 2.f);
}

SpriteBatch::~SpriteBatch()
{
	delete renderer;
}

Texture* SpriteBatch::Load(const wchar_t* filepath, float width, float height)
{
	return new Texture(renderer->LoadContent(filepath, height, width), width, height);
}

void SpriteBatch::UnloadTextures()
{
	renderer->ClearTextures();
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

void SpriteBatch::DrawUI(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot, ANCHOR_POINT anchor)
{
	Point<float> drawLocation = Point<float>(rectangle->X(), rectangle->Y());


	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::HCENTER)
	{
		drawLocation.x = ((float)windowWidth / 2.f) - (rectangle->Width() / 2.f) - drawLocation.x;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		drawLocation.x = windowWidth - rectangle->Width() - drawLocation.x;
	}

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::VCENTER)
	{
		drawLocation.y = ((float)windowHeight / 2.f) - (rectangle->Height() / 2.f) - drawLocation.y;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::BOTTOM)
	{
		drawLocation.y = windowHeight - drawLocation.y - rectangle->Height();
	}

	if (!source)
		renderer->Draw(texture->ID(),
			drawLocation.x, drawLocation.y, (int)rectangle->Width(), (int)rectangle->Height(),
			0.f, 0.f, (int)texture->Width(), (int)texture->Height(),
			opacity, layer, rot);
	else
		renderer->Draw(texture->ID(),
			drawLocation.x, drawLocation.y, (int)rectangle->Width(), (int)rectangle->Height(),
			source->X(), source->Y(), (int)source->Width(), (int)source->Height(),
			opacity, layer, rot);
}

void SpriteBatch::WriteText(const char* text, MyRectangle* rectangle, float layer, float opacity, ANCHOR_POINT anchor)
{
	Point<float> writeLocation = Point<float>(rectangle->X(), rectangle->Y());

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::HCENTER)
	{
		writeLocation.x = ((float)windowWidth / 2.f) - (rectangle->Width() / 2.f) - writeLocation.x;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		writeLocation.x = windowWidth - rectangle->Width() - writeLocation.x;
	}

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::VCENTER)
	{
		writeLocation.y = ((float)windowHeight / 2.f) - (rectangle->Height() / 2.f) - writeLocation.y;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::BOTTOM)
	{
		writeLocation.y = windowHeight - writeLocation.y - rectangle->Height();
	}

	renderer->Write(text, writeLocation.x, writeLocation.y, rectangle->Width(), rectangle->Height(), layer, opacity);
}

void SpriteBatch::WriteText(Text* text)
{
	WriteText(text->GetText().c_str(), text->Rectangle(), text->Layer(), text->Opacity(), text->Anchor());
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

