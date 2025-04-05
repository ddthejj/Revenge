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

Point<float> GetAbsolutePos(const MyRectangle* rectangle, const MyRectangle* sourceRectangle, ANCHOR_POINT anchor)
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

void SpriteBatch::Draw(const Sprite* sprite)
{
	const Texture* texture = sprite->GetTexture();
	const MyRectangle* rectangle = sprite->GetRectangle();
	const MyRectangle* source = sprite->GetSourceRectangle();
	float opacity = sprite->GetOpacity();
	float layer = sprite->GetLayer();
	int rot = sprite->GetRotation();;

	MyRectangle drawRectangle(*rectangle);
	drawRectangle.SetLocation(drawRectangle.Location() + Point<float>(windowWidth / 2.f, windowHeight / 2.f));
	drawRectangle.SetLocation(drawRectangle.Location() - Point<float>(camera[0], camera[1]));
	drawRectangle.SetLocation(drawRectangle.Location() + (Point<float>(windowWidth / 2.f, windowHeight / 2.f) - drawRectangle.Location()) * (1.f - camera[2]));

	drawRectangle.SetWidth(drawRectangle.Width() * camera[2]);
	drawRectangle.SetHeight(drawRectangle.Height() * camera[2]);

	if (!source)
	{
		if (texture->SourceRectangle())
		{
			renderer->Draw(
				texture->ID(),
				drawRectangle.X(),
				drawRectangle.Y(),
				(int)drawRectangle.Width(),
				(int)drawRectangle.Height(),
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
				drawRectangle.X(),
				drawRectangle.Y(),
				(int)drawRectangle.Width(),
				(int)drawRectangle.Height(),
				0.f, 0.f, (int)texture->Width(), (int)texture->Height(),
				opacity, layer, rot);
		}
	}
	else
	{
		if (texture->SourceRectangle())
		{
			renderer->Draw(texture->ID(),
				drawRectangle.X(),
				drawRectangle.Y(),
				(int)drawRectangle.Width(),
				(int)drawRectangle.Height(),
				source->X() + texture->SourceRectangle()->X(),
				source->Y() + texture->SourceRectangle()->Y(),
				(int)source->Height(),
				(int)source->Width(),
				opacity, layer, rot);
		}
		else
		{
			renderer->Draw(texture->ID(),
				drawRectangle.X(),
				drawRectangle.Y(),
				(int)drawRectangle.Width(),
				(int)drawRectangle.Height(),
				source->X(),
				source->Y(),
				(int)source->Height(),
				(int)source->Width(),
				opacity, layer, rot);
		}
	}
}

void SpriteBatch::DrawUI(const UISprite* sprite)
{
	const Texture* texture = sprite->GetTexture();
	const MyRectangle* rectangle = sprite->GetRectangle();
	const MyRectangle* source = sprite->GetSourceRectangle();
	float opacity = sprite->GetOpacity();
	float layer = sprite->GetLayer();
	int rot = sprite->GetRotation();;

	MyRectangle screnRec = MyRectangle(0, 0, (float)windowWidth, (float)windowHeight);
	Point<float> drawLocation = GetAbsolutePos(rectangle, &screnRec, sprite->GetAnchorPoint());

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

void SpriteBatch::DrawUIInSprite(const UISprite* drawSprite, const UISprite* sourceSprite)
{
	UISprite newSprite = UISprite(*drawSprite);
	MyRectangle screenRec = MyRectangle(0, 0, (float)windowWidth, (float)windowHeight);
	Point<float> sourceAbsolutePos = GetAbsolutePos(sourceSprite->GetRectangle(), &screenRec, sourceSprite->GetAnchorPoint());
	MyRectangle sourceAbsoluteRec = MyRectangle(sourceAbsolutePos, sourceSprite->GetRectangle()->Width(), sourceSprite->GetRectangle()->Height());
	Point<float> drawAbsolutePos = GetAbsolutePos(drawSprite->GetRectangle(), &sourceAbsoluteRec, drawSprite->GetAnchorPoint());
	newSprite.SetRectangle(MyRectangle(drawAbsolutePos, drawSprite->GetRectangle()->Width(), drawSprite->GetRectangle()->Height()));
	newSprite.SetAnchorPoint(ANCHOR_POINT::ANCHOR_TOP_LEFT);

	DrawUI(&newSprite);
}

void SpriteBatch::WriteText(const Text* text)
{
	const MyRectangle* rectangle = text->GetRectangle();
	ANCHOR_POINT anchor = text->GetAnchorPoint();

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

	renderer->Write(text->GetText().c_str(), writeLocation.x, writeLocation.y, rectangle->Width(), rectangle->Height(), text->GetLayer(), text->GetOpacity());
}

void SpriteBatch::WriteTextInSprite(const Text* text, const UISprite* sprite)
{
	const MyRectangle* textRectangle = text->GetRectangle();

	MyRectangle screenRec = MyRectangle(0, 0, (float)windowWidth, (float)windowHeight);
	MyRectangle sourceRectangle = MyRectangle(GetAbsolutePos(sprite->GetRectangle(), &screenRec, sprite->GetAnchorPoint()), sprite->GetRectangle()->Width(), sprite->GetRectangle()->Height());
	Point<float> sourceLocation = GetAbsolutePos(textRectangle, &sourceRectangle, text->GetAnchorPoint());
	MyRectangle absoluteRectangle = MyRectangle(sourceLocation, textRectangle->Width(), textRectangle->Height());

	renderer->Write(text->GetText().c_str(), sourceLocation.x, sourceLocation.y, textRectangle->Width(), textRectangle->Height(), text->GetLayer(), text->GetOpacity());
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

void SpriteBatch::SetCameraScale(float z)
{
	camera[2] = z;
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