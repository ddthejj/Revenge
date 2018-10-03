#pragma once

#include <Windows.h>
#include <d2d1.h>

class Texture;
class MyRectangle;

class SpriteBatch
{
	ID2D1Factory* factory = NULL;
	ID2D1HwndRenderTarget* renderTarget = NULL;
	ID2D1SolidColorBrush* brush = NULL;
public:
	SpriteBatch();
	~SpriteBatch();

	bool Init(HWND windowHandle);

	ID2D1RenderTarget *GetRenderTarget() { return renderTarget; }

	void Begin() { renderTarget->BeginDraw(); }
	void End() { renderTarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);
	void Draw(Texture* texture, MyRectangle rectangle, MyRectangle sourceRectangle, float transparency);
};