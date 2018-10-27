#pragma once
#include "defines.h"

using native_handle = void*;

class Texture;
class MyRectangle;
class Renderer;

class SpriteBatch
{
	Renderer* renderer;
	float camera[2];

public:
	SpriteBatch(native_handle hwnd);
	~SpriteBatch();
	Texture* Load(const wchar_t* filepath, float height, float width);
	void Begin();
	void Draw(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot = 0);
	void Draw(Texture* texture, MyRectangle* rectangle, float opacity, float layer, int rot = 0) { Draw(texture, rectangle, nullptr, opacity, layer, rot); }
	void DrawUI(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot = 0);
	void DrawUI(Texture* texture, MyRectangle* rectangle, float opacity, float layer, int rot = 0) { DrawUI(texture, rectangle, nullptr, opacity, layer, rot); }
	void WriteText(const wchar_t* text, MyRectangle* rectangle, float layer);
	void End();
	void MoveCamera(float x, float y);
	void SetCamera(float x, float y);
	void Resize(native_handle hwnd);
};

