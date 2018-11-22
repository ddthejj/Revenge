#pragma once
#include "defines.h"

#include <Windows.h>


class Renderer
{
	struct ToDraw;
	struct ObjectDraw;
	struct TextDraw;
	
	struct Impl_Elements;

	Impl_Elements* elements;

public:
	Renderer(HWND hwnd);
	~Renderer();

	bool Begin();
	bool Draw(unsigned int textureID, float x, float y, int width, int height, float x_S, float y_S, int width_S, int height_S, float opacity, float layer, int rot);
	bool Write(const char* text, float x, float y, float width, float height, float layer);
	bool End();

	int LoadContent(const wchar_t* filePath, float height, float width);
	bool Resize(HWND hwnd);
};