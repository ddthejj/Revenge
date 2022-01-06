#pragma once
#include "defines.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "Text.h"
#include "Sprite.h"

int SpriteBatch::windowWidth = (int)WIDTH;
int SpriteBatch::windowHeight = (int)HEIGHT;

SpriteBatch::SpriteBatch(native_handle hwnd)
{
	renderer = new Renderer((HWND)hwnd);

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
	MyRectangle screnRec = MyRectangle(0, 0, (float)windowWidth, (float)windowHeight);
	Point<float> drawLocation = GetAbsolutePos(rectangle, &screnRec, anchor);

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
		writeLocation.x = ((float)windowWidth / 2.f) - (rectangle->Width() / 2.f) + writeLocation.x;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		writeLocation.x = windowWidth - rectangle->Width() - writeLocation.x;
	}

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::VCENTER)
	{
		writeLocation.y = ((float)windowHeight / 2.f) - (rectangle->Height() / 2.f) + writeLocation.y;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::BOTTOM)
	{
		writeLocation.y = windowHeight - writeLocation.y - rectangle->Height();
	}

	renderer->Write(text, writeLocation.x, writeLocation.y, rectangle->Width(), rectangle->Height(), layer, opacity);
}

void SpriteBatch::DrawUIInSprite(Texture* texture, MyRectangle* rectangle, MyRectangle* source, UISprite* sourceSprite, float opacity, float layer, int rot, ANCHOR_POINT anchor)
{
	MyRectangle screenRec = MyRectangle(0, 0, (float)windowWidth, (float)windowHeight);	
	MyRectangle boundsRectangle = MyRectangle(GetAbsolutePos(sourceSprite->GetRectangle(), &screenRec, sourceSprite->GetAnchorPoint()), sourceSprite->GetRectangle()->Width(), sourceSprite->GetRectangle()->Height());
	Point<float> absoluteLocation = GetAbsolutePos(rectangle, &boundsRectangle, anchor);
	MyRectangle absoluteRectangle = MyRectangle(absoluteLocation, rectangle->Width(), rectangle->Height());
	DrawUI(texture, &absoluteRectangle, source, opacity, layer, rot, ANCHOR_POINT::ANCHOR_TOP_LEFT);
}

void SpriteBatch::WriteText(Text* text)
{
	WriteText(text->GetText().c_str(), text->Rectangle(), text->Layer(), text->Opacity(), text->Anchor());
}

void SpriteBatch::WriteTextInSprite(const char* text, MyRectangle* textRectangle, UISprite* sourceSprite, float layer, float opacity, ANCHOR_POINT anchor)
{
	MyRectangle screenRec = MyRectangle(0, 0, (float)windowWidth, (float)windowHeight);
	MyRectangle sourceRectangle = MyRectangle(GetAbsolutePos(sourceSprite->GetRectangle(), &screenRec, sourceSprite->GetAnchorPoint()), sourceSprite->GetRectangle()->Width(), sourceSprite->GetRectangle()->Height());
	Point<float> sourceLocation = GetAbsolutePos(textRectangle, &sourceRectangle, anchor);
	MyRectangle absoluteRectangle = MyRectangle(sourceLocation, textRectangle->Width(), textRectangle->Height());
	WriteText(text, &absoluteRectangle, layer, opacity, ANCHOR_POINT::ANCHOR_TOP_LEFT);
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

Point<float> SpriteBatch::GetAbsolutePos(const MyRectangle* rectangle, const MyRectangle* sourceRectangle, ANCHOR_POINT anchor)
{
	Point<float> drawLocation = Point<float>(rectangle->X(), rectangle->Y());

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::HCENTER)
	{
		drawLocation.x = ((float)sourceRectangle->Width() / 2.f) - (rectangle->Width() / 2.f) + drawLocation.x;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		drawLocation.x = sourceRectangle->Width() - rectangle->Width() - drawLocation.x;
	}
	drawLocation.x += sourceRectangle->X();

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::VCENTER)
	{
		drawLocation.y = ((float)sourceRectangle->Height() / 2.f) - (rectangle->Height() / 2.f) + drawLocation.y;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::BOTTOM)
	{
		drawLocation.y = sourceRectangle->Height() - drawLocation.y - rectangle->Height();
	}
	drawLocation.y += sourceRectangle->Y();

	return drawLocation;
}
